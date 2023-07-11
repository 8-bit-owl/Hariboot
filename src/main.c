#include <efi.h>
#include <efilib.h>

#include "graphics/gop.h"
#include "graphics/colors.h"
#include "graphics/drawing.h"
#include "io/output.h"
#include "memory/mem.h"


static void fillScreen(EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop, UINT32 BaseColor) {

    UINT32 width = Gop->Mode->Info->HorizontalResolution;
    UINT32 height = Gop->Mode->Info->VerticalResolution;
    UINT32 color = BaseColor;

    for (UINT32 i = 0; i < width; i++) {
        for (UINT32 j = 0; j < height; j++) {
            drawPoint_32bpp(Gop, (Vec2){i, j}, color);
            color++;
        }
    }
}


EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {

    EFI_PHYSICAL_ADDRESS *buffer;
    EFI_STATUS Status;
    EFI_INPUT_KEY Key;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION **ModeInfos;

    Status = setBackForeColor(SystemTable, VGA_RED, VGA_BLACK);
    resetTerm(SystemTable);
    Status = SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Coucou le monde\r\n");

    Gop = locateGOP(SystemTable);
    //ModeInfos = getModeInfos(SystemTable, Gop, 0x20);

    setVideoMode(SystemTable, Gop, 0x03d);
    fillScreen(Gop, 0xFF0000);

    while ((Status = SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key)) == EFI_NOT_READY);

    return EFI_SUCCESS;
}
