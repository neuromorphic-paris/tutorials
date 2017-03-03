#include <sepia.hpp>
#include <tarsier/maskIsolated.hpp>
#include <tarsier/computeFlow.hpp>
#include <chameleon/backgroundCleaner.hpp>
#include <chameleon/changeDetectionDisplay.hpp>
#include <chameleon/flowDisplay.hpp>
#include <QGuiApplication>
#include <QtQuick/QQuickView>

/// FlowEvent represents the parameters of an optical flow update.
struct FlowEvent {
    std::size_t x;
    std::size_t y;
    uint64_t timestamp;
    double vx;
    double vy;
};

int main(int argc, char *argv[]) {
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
    view.setSource(QUrl("qrc:/main.qml"));
    view.show();

    auto changeDetectionDisplay = view.rootObject()->findChild<chameleon::ChangeDetectionDisplay*>("changeDetectionDisplay");
    auto flowDisplay = view.rootObject()->findChild<chameleon::FlowDisplay*>("flowDisplay");

    auto computeOnEventsFlow = tarsier::make_computeFlow<sepia::ChangeDetection, FlowEvent, 304, 240, 2, 15, 1000000>(
        [](sepia::ChangeDetection changeDetection, double vx, double vy) -> FlowEvent {
            return FlowEvent{changeDetection.x, changeDetection.y, changeDetection.timestamp, vx, vy};
        },
        [flowDisplay](FlowEvent flowEvent) -> void {
            flowDisplay->push(flowEvent);
        }
    );
    auto computeOffEventsFlow = tarsier::make_computeFlow<sepia::ChangeDetection, FlowEvent, 304, 240, 2, 15, 1000000>(
        [](sepia::ChangeDetection changeDetection, double vx, double vy) -> FlowEvent {
            return FlowEvent{changeDetection.x, changeDetection.y, changeDetection.timestamp, vx, vy};
        },
        [flowDisplay](FlowEvent flowEvent) -> void {
            flowDisplay->push(flowEvent);
        }
    );

    auto eventStreamObservable = sepia::make_eventStreamObservable(
        "/Users/Bob/Desktop/recording.es",
        sepia::make_split(
            tarsier::make_maskIsolated<sepia::ChangeDetection, 304, 240, 1000>(
                [changeDetectionDisplay, &computeOnEventsFlow, &computeOffEventsFlow](sepia::ChangeDetection changeDetection) -> void {
                    changeDetectionDisplay->push(changeDetection);
                    if (changeDetection.isIncrease) {
                        computeOnEventsFlow(changeDetection);
                    } else {
                        computeOffEventsFlow(changeDetection);
                    }
                }
            ),
            [](sepia::ThresholdCrossing) {}
        ),
        [](std::exception_ptr) {},
        sepia::EventStreamObservable::Dispatch::synchronouslyButSkipOffset,
        []() -> bool {
            return true;
        }
    );

    return app.exec();
}
