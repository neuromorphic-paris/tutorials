#include "mySeriousEventHandler.hpp"

#include <sepia.hpp>
#include <chameleon/backgroundCleaner.hpp>
#include <chameleon/changeDetectionDisplay.hpp>
#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickView>
#include <QtCore/QFileInfo>

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);

    qmlRegisterType<chameleon::BackgroundCleaner>("BackgroundCleaner", 1, 0, "BackgroundCleaner");
    qmlRegisterType<chameleon::ChangeDetectionDisplay>("ChangeDetectionDisplay", 1, 0, "ChangeDetectionDisplay");

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QQuickView view(QUrl::fromLocalFile(QFileInfo("../../source/main.qml").absoluteFilePath()));
    view.setFormat(format);
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.show();

    auto changeDetectionDisplay = view.rootObject()->findChild<chameleon::ChangeDetectionDisplay*>("changeDetectionDisplay");

    auto atisEventStreamObservable = sepia::make_atisEventStreamObservable(
        "/Users/Bob/Desktop/recording.es",
        sepia::make_split(
            make_mySeriousEventHandler<sepia::DvsEvent, 304, 240>(
                2 * M_PI / 5e6,

                // This lambda function is a function handler as well!
                // It simulates an op art sphere.
                // Lambda functions are easier to write than standalone event handlers such as mySeriousEventHandler.
                // However they are not re-usable and require global variables to hold a state.
                // As such, lambda functions should be used only for fast prototyping or to push events to the Chameleon library's displays.
                [&](sepia::DvsEvent dvsEvent) -> void {
                    const auto xCenter = 152.0;
                    const auto yCenter = 120.0;
                    const auto xDelta = dvsEvent.x - xCenter;
                    const auto yDelta = dvsEvent.y - yCenter;
                    const auto expansion = std::exp(-std::hypot(xDelta, yDelta) / 120) + 1;
                    const auto x = xCenter + expansion * xDelta;
                    const auto y = yCenter + expansion * yDelta;
                    if (x < 304 && x >= 0 && y < 240 && y >= 0) {
                        dvsEvent.x = static_cast<std::size_t>(x);
                        dvsEvent.y = static_cast<std::size_t>(y);
                        dvsEventDisplay->push(dvsEvent);
                    }
                }
            ),
            [](sepia::ThresholdCrossing) -> void {}
        ),
        [](std::exception_ptr) {}
    );

    return app.exec();
}
