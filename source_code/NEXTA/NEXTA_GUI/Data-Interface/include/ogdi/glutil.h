#define GLHOME "SOFTWARE\\GRASSLAND\\HOME"
#define GLINFO "SOFTWARE\\GRASSLAND\\INFO"
#define APPLICATION_NAME "GRASSLAND"
#define APPLICATION_USER_HOME "USRHOME"
#define APPLICATION_USER_GISRC "GISRC"
#define APPLICATION_USER_GISBASE "GISBASE"



/* PROTOTYPE */
char *getGLenv(void);
char *getUSRHOMEenv(void);
char *getGISRCenv();
char *getGISBASEenv();
int remove_dir(char *path);
char *ConvertFStoBSString(char *in);
char *ConvertBStoFSString(char *in);

int bCreate_status_d_text(char *text1,char *text2);
int bUpdate_status_d_text(char *text1,char *text2);
int bIs_status_d_Cancel_checked();
void close_status_d();

