                    #include <stdio.h>
#include "../include/hpdf.h"

int main() {
    HPDF_Doc pdf = HPDF_New(NULL, NULL);
    if (!pdf) {
        printf("❌ Failed to create PDF\n");
        return 1;
    }

    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
    HPDF_Page_BeginText(page);
    HPDF_Page_SetFontAndSize(page, HPDF_GetFont(pdf, "Helvetica", NULL), 14);
    HPDF_Page_TextOut(page, 50, 750, "✅ PDF engine works inside AjCampus!");
    HPDF_Page_EndText(page);

    HPDF_SaveToFile(pdf, "test_output.pdf");
    HPDF_Free(pdf);
    printf("📤 Test PDF generated: test_output.pdf\n");
    return 0;
}
