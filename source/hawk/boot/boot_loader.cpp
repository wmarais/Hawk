#define EFI_DEBUG 0
#define EFI_DEBUG_CLEAR_MEMORY 0

#include <efi.h>
#include <efilib.h>

EFI_STATUS
EFIAPI
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
  InitializeLib(ImageHandle, SystemTable);
  Print(reinterpret_cast<const CHAR16*>(L"Hello, world!\n"));
  return EFI_SUCCESS;
}
