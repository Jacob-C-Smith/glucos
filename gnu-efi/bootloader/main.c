#include <efi.h>
#include <efilib.h>
#include <elf.h>

typedef struct {
	void               *p_base;
	unsigned long long  size;
	unsigned int        width;
	unsigned int        height;
	unsigned int        pixels_per_scanline;
} framebuffer;

int memcmp ( const void *source, const void *dest, unsigned size )
{

	// Initialized data
	const unsigned char *a = source,
	                    *b = dest;

	// Iterate over each byte of memory
	for ( unsigned i = 0; i < size; i++ )
	{
		if ( a[i] < b[i] )
			return -1;
		else if ( a[i] > b[i] )
			return 1;
	}

	return 0;	
}

UINTN strcmp ( char *a, char *b, UINTN len)
{
	for(UINTN i = 0; i < len; i++)
	{
		if (*a != *b) return 0;
	}

	return 1;
}

EFI_FILE *load_file ( EFI_FILE *p_directory, CHAR16 *path, EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *p_system_table )
{

	// Uninitialized data

	// Initialized data
	EFI_FILE                        *p_loaded_file  = 0;
	EFI_LOADED_IMAGE_PROTOCOL       *p_loaded_image = 0;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *p_file_system  = 0;
	EFI_STATUS                       status         = 0;

	p_system_table->BootServices->HandleProtocol(imageHandle,&gEfiLoadedImageProtocolGuid, (void **)&p_loaded_image);
	p_system_table->BootServices->HandleProtocol(p_loaded_image->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void **)&p_file_system);

	if ( p_directory == NULL )
		p_file_system->OpenVolume(p_file_system, &p_directory);

	status = p_directory->Open(p_directory,&p_loaded_file, path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);

	if (status != EFI_SUCCESS)
		goto failed_to_load_file;
	
	return p_loaded_file;
	failed_to_load_file:return 0;
}

EFI_STATUS efi_main ( EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *p_system_table )
{
	
	// Initialized data
	Elf64_Ehdr               header            = { 0 };
	EFI_FILE                *p_kernel_efi_file = 0;
	EFI_CONFIGURATION_TABLE *p_config_table    = 0;
	EFI_GUID                 ACPI2_table_guid  = ACPI_20_TABLE_GUID;
	Elf64_Phdr              *phdrs             = 0;
	framebuffer              fb                = { 0 };
	void                    *rsdp = 0;

	// Initialize UEFI
	InitializeLib(image_handle, p_system_table);
	
	p_config_table = p_system_table->ConfigurationTable;

	for (UINTN i = 0; i < p_system_table->NumberOfTableEntries; i++)
	{
		if (CompareGuid(&p_config_table[i].VendorGuid, &ACPI2_table_guid))
		{
			if (strcmp((char*)"RSD PTR ", (char *)p_config_table->VendorTable, 8))
			{
				rsdp = (void *)p_config_table->VendorTable;
			}
		}
		p_config_table++;
	}
	

	p_kernel_efi_file = load_file(NULL,L"kernel.elf", image_handle, p_system_table);

	// Load the kernel file
	if ( p_kernel_efi_file == NULL )
		goto failed_to_load_kernel; 
	else
		Print(L"Kernel file loaded succeessfully!\r\n");

	// Load the file into memory
	{

		// Initialized data
		UINTN          size        = sizeof(Elf64_Ehdr);
		UINTN          file_info_size;
		EFI_FILE_INFO *p_file_info;

		// Get the size of the file
		p_kernel_efi_file->GetInfo(p_kernel_efi_file, &gEfiFileInfoGuid, &file_info_size, NULL);

		// Allocate memory for the file load info
		p_system_table->BootServices->AllocatePool(EfiLoaderData, file_info_size, (void **)&p_file_info);

		// Get the file info
		p_kernel_efi_file->GetInfo(p_kernel_efi_file, &gEfiFileInfoGuid, &file_info_size, (void**) &p_file_info);

		// Read the file into memory
		p_kernel_efi_file->Read(p_kernel_efi_file, &size, &header);
	}
	Print(L"Verifying kernel header!\r\n");
	if(
		memcmp(&header.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0 ||
	    header.e_ident[EI_CLASS] != ELFCLASS64  ||
		header.e_ident[EI_DATA]  != ELFDATA2LSB || 
		header.e_type            != ET_EXEC     ||
		header.e_machine         != EM_X86_64   ||
		header.e_version         != EV_CURRENT 
	)
		goto bad_kernel_format;
	else
		Print(L"Kernel header verified!\r\n");
	// Get the ELF headers
	{

		// Initialized data
		UINTN size = header.e_phnum * header.e_phentsize;

		// Set the file cursor to the correct offset
		p_kernel_efi_file->SetPosition(p_kernel_efi_file, header.e_phoff);
		
		p_system_table->BootServices->AllocatePool(EfiLoaderData, size, (void **)&phdrs);

		p_kernel_efi_file->Read(p_kernel_efi_file, &size, phdrs);

	}

	for (
		Elf64_Phdr *phdr = phdrs;
		(char *)phdr < (char *)phdrs + header.e_phnum * header.e_phentsize;
		phdr = (Elf64_Phdr*)((char *)phdr + header.e_phentsize)
	)
	{
		switch (phdr->p_type)
		{
			case PT_LOAD:
			{
				int pages = (phdr->p_memsz + 0x1000 - 1) / 0x1000;
				Elf64_Addr segment = phdr->p_paddr;
				UINTN size = 0;

				p_system_table->BootServices->AllocatePages(AllocateAddress, EfiLoaderData, pages, &segment);

				p_kernel_efi_file->SetPosition(p_kernel_efi_file, phdr->p_offset);
				size = phdr->p_filesz;
				p_kernel_efi_file->Read(p_kernel_efi_file, &size, (void *)segment);
				break;
			}
		}
	}

	Print(L"Kernel loaded!\r\n");

	Print(L"Entry point: 0x%x\r\n", header.e_entry);

	{
		EFI_GUID                      gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
		EFI_GRAPHICS_OUTPUT_PROTOCOL *p_gop    = 0;
		EFI_STATUS                    status   = 0;

		status = uefi_call_wrapper(BS->LocateProtocol, 3, &gop_guid, NULL, (void **)&p_gop);

		if ( EFI_ERROR(status) )
			goto unable_to_locate_gop;
		else
			Print(L"Found GOP\r\n");

		fb = (framebuffer) {
			.p_base = (void *) p_gop->Mode->FrameBufferBase,
			.size = p_gop->Mode->FrameBufferSize,
			.width = p_gop->Mode->Info->HorizontalResolution,
			.height = p_gop->Mode->Info->VerticalResolution,
			.pixels_per_scanline = p_gop->Mode->Info->PixelsPerScanLine
		};

		Print(L"base:   0x%x\n\r",fb.p_base);
		Print(L"size:   0x%x\n\r",fb.size);
		Print(L"width:  %d\n\r",fb.width);
		Print(L"height: %d\n\r",fb.height);
		Print(L"rppsl:  %d\n\r",fb.pixels_per_scanline);
		Print(L"vres:   %d\n\r",p_gop->Mode->Info->VerticalResolution);
		Print(L"hres:   %d\n\r",p_gop->Mode->Info->HorizontalResolution);
		
	}

	int (*kernel_start)(framebuffer *p_fb, void *rsdp) = ((__attribute__((sysv_abi)) int (*)(framebuffer *, void *)) header.e_entry);

	int r = kernel_start(&fb, rsdp);

	Print(L"Kernel returned %d\r\n", r);

	// Exit
	return EFI_SUCCESS;
	failed_to_load_kernel:
		Print(L"[glucos] [bootloader] Failed to load kernel!\r\n");
		return 1;
	bad_kernel_format:
		Print(L"[glucos] [bootloader] Kernel ELF file was incorrectly formatted!\r\n");
		return 1;
	unable_to_locate_gop:
		Print(L"[glucos] [bootloader] Unable to loacte graphics output protocol\r\n");
		return 1;
}
