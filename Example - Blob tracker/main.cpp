#include <sepia.hpp>
#include <tarsier/maskIsolated.hpp>
#include <tarsier/replicate.hpp>
#include <tarsier/trackBlobs.hpp>
#include <chameleon/backgroundCleaner.hpp>
#include <chameleon/changeDetectionDisplay.hpp>
#include <chameleon/blobDisplay.hpp>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    qmlRegisterType<chameleon::BackgroundCleaner>("BackgroundCleaner", 1, 0, "BackgroundCleaner");
    qmlRegisterType<chameleon::ChangeDetectionDisplay>("ChangeDetectionDisplay", 1, 0, "ChangeDetectionDisplay");
    qmlRegisterType<chameleon::BlobDisplay>("BlobDisplay", 1, 0, "BlobDisplay");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:/main.qml"));
    view.show();

    auto changeDetectionDisplay = view.rootObject()->findChild<chameleon::ChangeDetectionDisplay*>("changeDetectionDisplay");
    auto blobDisplay = view.rootObject()->findChild<chameleon::BlobDisplay*>("blobDisplay");
    auto hiddenBlobDisplay = view.rootObject()->findChild<chameleon::BlobDisplay*>("hiddenBlobDisplay");

    auto eventStreamObservable = sepia::make_eventStreamObservable(
        "/Users/Alex/idv/libraries/sepia/test/sepiaTest.es",
        sepia::make_split(
            tarsier::make_maskIsolated<sepia::ChangeDetection>(
                304,
                240,
                10000,
                tarsier::make_replicate<sepia::ChangeDetection>(
                    tarsier::make_trackBlobs<sepia::ChangeDetection>(
                        {
                            tarsier::Blob{38 * 1, 40 * 1, 250, 0, 250},
                            tarsier::Blob{38 * 3, 40 * 1, 250, 0, 250},
                            tarsier::Blob{38 * 5, 40 * 1, 250, 0, 250},
                            tarsier::Blob{38 * 7, 40 * 1, 250, 0, 250},
                            tarsier::Blob{38 * 1, 40 * 3, 250, 0, 250},
                            tarsier::Blob{38 * 3, 40 * 3, 250, 0, 250},
                            tarsier::Blob{38 * 5, 40 * 3, 250, 0, 250},
                            tarsier::Blob{38 * 7, 40 * 3, 250, 0, 250},
                            tarsier::Blob{38 * 1, 40 * 5, 250, 0, 250},
                            tarsier::Blob{38 * 3, 40 * 5, 250, 0, 250},
                            tarsier::Blob{38 * 5, 40 * 5, 250, 0, 250},
                            tarsier::Blob{38 * 7, 40 * 5, 250, 0, 250},
                        },
                          1e5, // activityDecay
                            0, // minimumProbability
                            1, // promotionActivity
                          0.1, // deletionActivity
                         0.99, // meanInertia
                         0.99, // covarianceInertia
                            1, // repulsionStrength
                           20, // repulsionLength
                         0.05, // attractionStrength
                          100, // attractionResetDistance
                        10000, // pairwiseCalculationsToSkip
                        [blobDisplay](std::size_t id, const tarsier::Blob& blob) {
                            blobDisplay->promoteBlob(id, blob);
                        },
                        [blobDisplay](std::size_t id, const tarsier::Blob& blob) {
                            blobDisplay->updateBlob(id, blob);
                        },
                        [blobDisplay](std::size_t id, const tarsier::Blob& blob) {
                            blobDisplay->demoteBlob(id, blob);
                        },
                        [hiddenBlobDisplay](std::size_t id, const tarsier::Blob& blob) {
                            hiddenBlobDisplay->promoteBlob(id, blob);
                        },
                        [hiddenBlobDisplay](std::size_t id, const tarsier::Blob& blob) {
                            hiddenBlobDisplay->updateBlob(id, blob);
                        },
                        [hiddenBlobDisplay](std::size_t id, const tarsier::Blob& blob) {
                            hiddenBlobDisplay->demoteBlob(id, blob);
                        },
                        [blobDisplay, hiddenBlobDisplay](std::size_t id, const tarsier::Blob& blob) {
                            blobDisplay->deleteBlob(id, blob);
                            hiddenBlobDisplay->deleteBlob(id, blob);
                        }
                    ),
                    [changeDetectionDisplay](sepia::ChangeDetection changeDetection) -> void {
                        changeDetectionDisplay->push(changeDetection);
                    }
                )
            ),
            [](sepia::ThresholdCrossing) -> void {}
        ),
        [](std::exception_ptr) {}
    );

    return app.exec();
}
