#include <sepia.hpp>
#include <tarsier/stitch.hpp>
#include <chameleon/backgroundCleaner.hpp>
#include <chameleon/changeDetectionDisplay.hpp>
#include <chameleon/logarithmicDisplay.hpp>
#include <chameleon/frameGenerator.hpp>
#include <QGuiApplication>
#include <QtQuick/QQuickView>

#include <iostream>

/// ExposureMeasurement holds the parameters of an exposure measurement.
struct ExposureMeasurement {
    size_t x;
    size_t y;
    uint64_t timeDelta;
};

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    qmlRegisterType<chameleon::BackgroundCleaner>("BackgroundCleaner", 1, 0, "BackgroundCleaner");
    qmlRegisterType<chameleon::ChangeDetectionDisplay>("ChangeDetectionDisplay", 1, 0, "ChangeDetectionDisplay");
    qmlRegisterType<chameleon::LogarithmicDisplay>("LogarithmicDisplay", 1, 0, "LogarithmicDisplay");
    qmlRegisterType<chameleon::FrameGenerator>("FrameGenerator", 1, 0, "FrameGenerator");

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
    auto logarithmicDisplay = view.rootObject()->findChild<chameleon::LogarithmicDisplay*>("logarithmicDisplay");
    auto frameGenerator = view.rootObject()->findChild<chameleon::FrameGenerator*>("frameGenerator");

    uint64_t firstTimestamp = 0;
    auto firstTimestampSet = false;
    std::size_t frameIndex = 0;

    auto eventStreamObservable = sepia::make_eventStreamObservable(
        "/Users/Bob/Desktop/recording.es",
        sepia::make_split(
            [
                firstTimestamp,
                firstTimestampSet,
                frameIndex,
                changeDetectionDisplay,
                logarithmicDisplay,
                frameGenerator
            ](sepia::ChangeDetection changeDetection) mutable -> void {
                if (!firstTimestampSet) {
                    firstTimestampSet = true;
                    firstTimestamp = changeDetection.timestamp;
                }
                if (changeDetection.timestamp - firstTimestamp >= 20000 * frameIndex) {
                    frameGenerator->saveFrameTo(std::string("/Users/Bob/Desktop/frames/") + std::to_string(frameIndex) + ".png");
                    const auto discards = logarithmicDisplay->discards();
                    std::cout << "Frame " << frameIndex << ": black discard: " << discards.x() << ", " << "whiteDiscard: " << discards.y() << std::endl;
                    ++frameIndex;
                }
                changeDetectionDisplay->push(changeDetection);
            },
            tarsier::make_stitch<sepia::ThresholdCrossing, ExposureMeasurement, 304, 240>(
                [](sepia::ThresholdCrossing secondThresholdCrossing, uint64_t timeDelta) -> ExposureMeasurement {
                    return ExposureMeasurement{secondThresholdCrossing.x, secondThresholdCrossing.y, timeDelta};
                },
                [logarithmicDisplay](ExposureMeasurement exposureMeasurement) -> void {
                    logarithmicDisplay->push(exposureMeasurement);
                }
            )
        ),
        [](std::exception_ptr) {}
    );

    return app.exec();
}
