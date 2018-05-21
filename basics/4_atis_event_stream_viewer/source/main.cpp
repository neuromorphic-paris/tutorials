#include <QtCore/QFileInfo>
#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickView>
#include <chameleon/backgroundCleaner.hpp>
#include <chameleon/changeDetectionDisplay.hpp>
#include <chameleon/logarithmicDisplay.hpp>
#include <sepia.hpp>
#include <tarsier/stitch.hpp>

/// ExposureMeasurement holds the parameters of an exposure measurement.
struct ExposureMeasurement {
    std::size_t x;
    std::size_t y;
    uint64_t timeDelta;
};

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);

    qmlRegisterType<chameleon::BackgroundCleaner>("BackgroundCleaner", 1, 0, "BackgroundCleaner");
    qmlRegisterType<chameleon::ChangeDetectionDisplay>("ChangeDetectionDisplay", 1, 0, "ChangeDetectionDisplay");
    qmlRegisterType<chameleon::LogarithmicDisplay>("LogarithmicDisplay", 1, 0, "LogarithmicDisplay");

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QQuickView view(QUrl::fromLocalFile(QFileInfo("../../source/main.qml").absoluteFilePath()));
    view.setFormat(format);
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.show();

    auto changeDetectionDisplay =
        view.rootObject()->findChild<chameleon::ChangeDetectionDisplay*>("changeDetectionDisplay");
    auto logarithmicDisplay = view.rootObject()->findChild<chameleon::LogarithmicDisplay*>("logarithmicDisplay");

    auto atisEventStreamObservable = sepia::make_atisEventStreamObservable(
        "/Users/Alex/idv/reallyGoodBidule/figures/test12.es",
        sepia::make_split(
            [&](sepia::DvsEvent dvsEvent) -> void { changeDetectionDisplay->push(dvsEvent); },
            tarsier::make_stitch<sepia::ThresholdCrossing, ExposureMeasurement, 304, 240>(
                [](sepia::ThresholdCrossing secondThresholdCrossing, uint64_t timeDelta) -> ExposureMeasurement {
                    return ExposureMeasurement{secondThresholdCrossing.x, secondThresholdCrossing.y, timeDelta};
                },
                [&](ExposureMeasurement exposureMeasurement) -> void {
                    logarithmicDisplay->push(exposureMeasurement);
                })),
        [](std::exception_ptr) {},
        []() { return true; });

    return app.exec();
}
