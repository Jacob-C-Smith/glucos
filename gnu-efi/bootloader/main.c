#include <efi.h>
#include <efilib.h>
#include <elf.h>

int memcmp(const void *source, const void *dest, unsigned size)
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

EFI_FILE *load_file(EFI_FILE *p_directory, CHAR16 *path, EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *p_system_table)
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

EFI_STATUS efi_main (EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *p_system_table)
{
	
	// Initialized data
	Elf64_Ehdr     header            = { 0 };
	EFI_FILE      *p_kernel_efi_file = 0;
	unsigned long  file_info_size    = 0;
	Elf64_Phdr    *phdrs             = 0;

	// Initialize UEFI
	InitializeLib(image_handle, p_system_table);

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

	Print(L"Kernel loaded !\r\n");

	Print(L"Entry point: 0x%x\r\n", header.e_entry);

	int (*kernel_start)() = ((__attribute__((sysv_abi)) int (*)()) header.e_entry);

	int r = kernel_start();

	Print(L"Kernel returned %d\r\n", r);

	// Exit
	return EFI_SUCCESS;
	failed_to_load_kernel:
		Print(L"Failed to load kernel!\r\n");
		return 1;
	bad_kernel_format:
		Print(L"ELF file was incorrectly formatted!\r\n");
		return 1;
}
