#include <sepia.hpp>
#include <tarsier/maskIsolated.hpp>
#include <tarsier/computeFlow.hpp>
#include <chameleon/backgroundCleaner.hpp>
#include <chameleon/changeDetectionDisplay.hpp>
#include <chameleon/flowDisplay.hpp>
#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickView>

/// FlowEvent represents the parameters of an optical flow update.
struct FlowEvent {
    std::size_t x;
    std::size_t y;
    uint64_t timestamp;
    double vx;
    double vy;
};

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);

    qmlRegisterType<chameleon::BackgroundCleaner>("BackgroundCleaner", 1, 0, "BackgroundCleaner");
    qmlRegisterType<chameleon::ChangeDetectionDisplay>("ChangeDetectionDisplay", 1, 0, "ChangeDetectionDisplay");
    qmlRegisterType<chameleon::FlowDisplay>("FlowDisplay", 1, 0, "FlowDisplay");

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QQuickView view;
    view.setFormat(format);
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl::fromLocalFile("../../source/main.qml"));
    view.show();

    auto changeDetectionDisplay = view.rootObject()->findChild<chameleon::ChangeDetectionDisplay*>("changeDetectionDisplay");
    auto flowDisplay = view.rootObject()->findChild<chameleon::FlowDisplay*>("flowDisplay");

    auto computeOnEventsFlow = tarsier::make_computeFlow<sepia::DvsEvent, FlowEvent, 304, 240, 2, 15, 1000000>(
        [](sepia::DvsEvent changeDetection, double vx, double vy) -> FlowEvent {
            return FlowEvent{changeDetection.x, changeDetection.y, changeDetection.timestamp, vx, vy};
        },
        [&](FlowEvent flowEvent) -> void {
            flowDisplay->push(flowEvent);
        }
    );
    auto computeOffEventsFlow = tarsier::make_computeFlow<sepia::DvsEvent, FlowEvent, 304, 240, 2, 15, 1000000>(
        [](sepia::DvsEvent changeDetection, double vx, double vy) -> FlowEvent {
            return FlowEvent{changeDetection.x, changeDetection.y, changeDetection.timestamp, vx, vy};
        },
        [&](FlowEvent flowEvent) -> void {
            flowDisplay->push(flowEvent);
        }
    );

    auto atisEventStreamObservable = sepia::make_atisEventStreamObservable(
        "/Users/Bob/Desktop/recording.es",
        sepia::make_split(
            tarsier::make_maskIsolated<sepia::DvsEvent, 304, 240, 1000>(
                [&](sepia::DvsEvent dvsEvent) -> void {
                    changeDetectionDisplay->push(dvsEvent);
                    if (dvsEvent.isIncrease) {
                        computeOnEventsFlow(dvsEvent);
                    } else {
                        computeOffEventsFlow(dvsEvent);
                    }
                }
            ),
            [](sepia::ThresholdCrossing) {}
        ),
        [](std::exception_ptr) {}
    );

    return app.exec();
}
