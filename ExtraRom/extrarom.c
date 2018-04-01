
typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;
typedef s32 Result;
typedef signed char bool;

#define USECACHE 1
#define ROMFS_HEADER_SIZE 0x20000 - 0x28
#define MAX_FILE_NUM 0x14
#define MAX_DIR_NUM 0x14
#define WORKING_MEM_SIZE MAX_FILE_NUM * 0x20 + MAX_DIR_NUM * 0x18 + (USECACHE ? ROMFS_HEADER_SIZE : 0)
#define ROMFS_ARCHIVE_OPENFILE_SUCCESS 0

typedef Result (*ContentRomFsArchive$$Create_t)(void **, int, int, void *, int, bool, u32 *);
typedef s32 (*RomFsArchive$$OpenFile_t)(void*, void**, u32*, u32);
static const ContentRomFsArchive$$Create_t ContentRomFsArchive$$Create = *(ContentRomFsArchive$$Create_t)0x117500;
static const RomFsArchive$$OpenFile_t RomFsArchive$$OpenFile = *(RomFsArchive$$OpenFile_t)0x001888A8;
static const u32 lowPath[3] = { 5, 0, 0 };
static const u8 *workingMemory = (u8*)0x0049ab98;

s32 hook_RomFsArchive$$OpenFile(void * thisptr,void ** ifile,u32 * pathObject, u32 mode)
{
	static void * romfsObject = 0;
	if (!romfsObject)
	{
		ContentRomFsArchive$$Create(&romfsObject, MAX_FILE_NUM, MAX_DIR_NUM, workingMemory, WORKING_MEM_SIZE, 1, lowPath);
	}
	if(romfsObject)
	{
		if(RomFsArchive$$OpenFile(romfsObject, ifile, pathObject, mode) == ROMFS_ARCHIVE_OPENFILE_SUCCESS) 
			return ROMFS_ARCHIVE_OPENFILE_SUCCESS;
	}
	return RomFsArchive$$OpenFile(thisptr, ifile, pathObject, mode);
}