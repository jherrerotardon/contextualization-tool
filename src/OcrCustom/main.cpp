#include <iostream>
#include <leptonica/allheaders.h>
#include "baseapi.h"

using namespace std;

int main(int argc, char **argv)
{
    char *outText;

    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();

    if (api->Init(
        "/home/jorge/Projects/contextualization-tool/tesseract/tessdata/",
        "eng",
        tesseract::OEM_TESSERACT_CUBE_COMBINED
        )
    ) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }

    Pix *image = pixRead(argv[1]);

    api->SetPageSegMode(tesseract::PSM_AUTO_OSD);
    api->SetImage(image);

    // OCR result
    outText = api->GetUTF8Text();
    cout << outText << endl;

    // Destroy used object and release memory
    api->End();
    delete [] outText;
    pixDestroy(&image);

    return 0;
}
