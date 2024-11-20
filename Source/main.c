#define NULL 0
struct EFI_SYSTEM_TABLE                *ST;
struct EFI_BOOT_SERVICES               *BS;
struct EFI_GRAPHICS_OUTPUT_PROTOCOL    *GOP;
struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SFSP;
struct EFI_LOADED_IMAGE_PROTOCOL       *LIP;
void efi_init(struct EFI_SYSTEM_TABLE *SystemTable) {
  ST = SystemTable;
  BS = SystemTable->BootServices;

  // 初始化
  BS->SetWatchdogTimer(0, 0, 0, NULL); // 别删，不然UEFI会自动重启
}
struct EFI_INPUT_KEY {
    unsigned short ScanCode;
    unsigned short UnicodeChar;
};

struct EFI_SYSTEM_TABLE {
    char _buf1[44];
    struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL {
        unsigned long long _buf;
        unsigned long long (*ReadKeyStroke)(
            struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This,
            struct EFI_INPUT_KEY *Key);
    } *ConIn;
    unsigned long long _buf2;
    struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
        unsigned long long _buf;
        unsigned long long (*OutputString)(
            struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
            unsigned short *String);
        unsigned long long _buf2[4];
        unsigned long long (*ClearScreen)(
            struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This);
    } *ConOut;
};

void efi_main(void *ImageHandle __attribute__ ((unused)),
          struct EFI_SYSTEM_TABLE *SystemTable)
{
    struct EFI_INPUT_KEY key;
    unsigned short str[3];
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"OpenMPKernel UEFI Bootloader (Verison 1.0.1)\r\n");
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Updated content:\r\n");
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"\r\n");
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Keyboard input\r\n");
    while (1) {
        if (!SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &key)) {
            if (key.UnicodeChar != L'\r') {
                str[0] = key.UnicodeChar;
                str[1] = L'\0';
            } else {
                str[0] = L'\r';
                str[1] = L'\n';
                str[2] = L'\0';
            }
            SystemTable->ConOut->OutputString(SystemTable->ConOut, str);
        }
    }
}