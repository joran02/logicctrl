#include "cmn.h"


#define ETC_MAXLINE             1024

#define ETC_FILENOTFOUND        -1
#define ETC_SECTIONNOTFOUND     -2
#define ETC_KEYNOTFOUND         -3
#define ETC_TMPFILEFAILED       -4
#define ETC_FILEIOFAILED        -5
#define ETC_INTCONV             -6
#define ETC_INVALIDOBJ          -7
#define ETC_READONLYOBJ         -8
#define ETC_OK                  0


/*int get_key_value (char *key_line, char **mykey, char **myvalue)
{
    char* current;
    char* tail;
    char* value;

    if (!key_line)
        return -1;

    current = key_line;

    while (*current == ' ' ||  *current == '\t') current++; 

    if (*current == ';' || *current == '#')
        return -1;

    if (*current == '[')
        return 1;

    if (*current == '\n' || *current == '\0' || *current == '\r')
        return -1;

    tail = current;
    while (*tail != '=' && *tail != '\n' &&
          *tail != ';' && *tail != '#' && *tail != '\0' && *tail != '\r')
          tail++;

    value = tail + 1;
    if (*tail != '=')
        *value = '\0'; 

    *tail-- = '\0';
    while (*tail == ' ' || *tail == '\t') {
        *tail = '\0';
        tail--; 
    }
        
    tail = value;
    while (*tail != '\n' && *tail != '\0' && *tail != '\r') tail++;
    *tail = '\0'; 

    if (mykey)
        *mykey = current;
    if (myvalue)
        *myvalue = value;

    return 0;
}*/

int etc_LocateKeyValue(FILE* fp, const char* pKey, 
                               U8 bCurSection, char* pValue, int iLen,
                               FILE* bak_fp, char* nextSection)
{
    char szBuff[ETC_MAXLINE + 1 + 1];
    char* current;
    char* value;
    int ret;

    while (1) {
        int bufflen;

        if (!fgets(szBuff, ETC_MAXLINE, fp))
            return ETC_FILEIOFAILED;
        bufflen = strlen (szBuff);
        if (szBuff [bufflen - 1] == '\n' || szBuff [bufflen - 1] == '\r')
            szBuff [bufflen - 1] = '\0';

        ret = get_key_value (szBuff, &current, &value);
        if (ret < 0)
            continue;
        else if (ret > 0) {
            fseek (fp, -bufflen, SEEK_CUR);
            return ETC_KEYNOTFOUND;
        }
            
        if (strcmp (current, pKey) == 0) {
            if (pValue)
                strncpy (pValue, value, iLen);

            return ETC_OK; 
        }
        else if (bak_fp && *current != '\0') {

 //           char tmp_nam[256];
    //        sprintf (tmp_nam , "%s=%s\n", pKey, pValue);
       //     fputs (tmp_nam, bak_fp);
            fprintf (bak_fp, "%s=%s\n", current, value);
        }
    }

    return ETC_KEYNOTFOUND;
}

/*char* get_section_name (char *section_line)
{
    char* current;
    char* name;

    if (!section_line)
        return NULL;

    current = section_line;

    while (*current == ' ' ||  *current == '\t') current++; 

    if (*current == ';' || *current == '#')
        return NULL;

    if (*current++ == '[')
        while (*current == ' ' ||  *current == '\t') current ++;
    else
        return NULL;

    name = current;
    while (*current != ']' && *current != '\n' &&
          *current != ';' && *current != '#' && *current != '\0' && *current != '\r')
          current++;
    *current = '\0';
    while (*current == ' ' || *current == '\t') {
        *current = '\0';
        current--; 
    }

    return name;
}

int etc_LocateSection(FILE* fp, const char* pSection, FILE* bak_fp)
{
    char szBuff[ETC_MAXLINE + 1];
    char *name;

    while (1) {
        if (!fgets(szBuff, ETC_MAXLINE, fp)) {
            if (feof (fp))
                return ETC_SECTIONNOTFOUND;
            else
                return ETC_FILEIOFAILED;
        }
        else if (bak_fp && fputs (szBuff, bak_fp) == EOF)
            return ETC_FILEIOFAILED;
        
        name = get_section_name (szBuff);
        if (!name)
            continue;

        if (strcmp (name, pSection) == 0)
            return ETC_OK; 
    }

    return ETC_SECTIONNOTFOUND;
}

int GetValueFromEtcFile(const char* pEtcFile, const char* pSection,
                               const char* pKey, char* pValue, int iLen)
{
	FILE* fp;
	char tempSection [ETC_MAXLINE + 2];

	if (!(fp = fopen(pEtcFile, "r")))
		return ETC_FILENOTFOUND;

	if (pSection)
	{
		if (etc_LocateSection (fp, pSection, NULL) != ETC_OK) 
		{
			fclose (fp);
			return ETC_SECTIONNOTFOUND;
		}

		if (etc_LocateKeyValue (fp, pKey, pSection != NULL, pValue, iLen, NULL, tempSection) != ETC_OK) 
		{
			fclose (fp);
			return ETC_KEYNOTFOUND;
		}
	}
	fclose (fp);
	return ETC_OK;
}*/

void __mg_rewind (FILE *fp)
{
	rewind (fp);
}

FILE* __mg_tmpfile (void)
{
	return fopen ("/tmp/mg-etc-tmp", "w+");  
}

int __mg_close_tmpfile (FILE *tmp_fp)
{    
	return fclose (tmp_fp);
}

static int etc_CopyAndLocate (FILE* etc_fp, FILE* tmp_fp, const char* pSection, const char* pKey, char* tempSection)
{    
	if (pSection && etc_LocateSection (etc_fp, pSection, tmp_fp) != ETC_OK)       
		return ETC_SECTIONNOTFOUND;    
	if (etc_LocateKeyValue (etc_fp, pKey, pSection != NULL, NULL, 0, tmp_fp, tempSection) != ETC_OK)        
		return ETC_KEYNOTFOUND;    
	return ETC_OK;
}

static int etc_FileCopy (FILE* sf, FILE* df)
{    
	char line [ETC_MAXLINE + 1];       
	
	while (fgets (line, ETC_MAXLINE + 1, sf) != NULL)        
	if (fputs (line, df) == EOF) 
	{            
		return ETC_FILEIOFAILED;        
	}    
	return ETC_OK;
}



int SetValueToEtcFile (const char* pEtcFile, const char* pSection, const char* pKey, char* pValue)
{   
	FILE* etc_fp;    
	FILE* tmp_fp;    
	int rc;    
	char tempSection [ETC_MAXLINE + 2] = {0};    

	if ((tmp_fp = __mg_tmpfile ()) == NULL)        
		return ETC_TMPFILEFAILED;    

	if (!(etc_fp = fopen (pEtcFile, "r+"))) 
	{        
		__mg_close_tmpfile (tmp_fp);        
		if (!(etc_fp = fopen (pEtcFile, "w"))) 
		{            
			return ETC_FILEIOFAILED;        
		}        
		fprintf (etc_fp, "[%s]\n", pSection);        
		fprintf (etc_fp, "%s=%s\n", pKey, pValue);        
		fclose (etc_fp);        
		return ETC_OK;    
	}    

	switch (etc_CopyAndLocate (etc_fp, tmp_fp, pSection, pKey, tempSection)) 
	{    
		case ETC_SECTIONNOTFOUND:        
			fprintf (tmp_fp, "\n[%s]\n", pSection);        
			fprintf (tmp_fp, "%s=%s\n", pKey, pValue);        
			break;        
		case ETC_KEYNOTFOUND:        
			fprintf (tmp_fp, "%s=%s\n", pKey, pValue);        
			fprintf (tmp_fp, "%s\n", tempSection);    
			break;    
		default:
			fprintf (tmp_fp, "%s=%s\n", pKey, pValue);
			break;    
	}    
	if ((rc = etc_FileCopy (etc_fp, tmp_fp)) != ETC_OK)        
		goto error;        
	// replace etc content with tmp file content    
	// truncate etc content first    fclose (etc_fp);    
	if (!(etc_fp = fopen (pEtcFile, "w"))) 
	{        
		__mg_close_tmpfile (tmp_fp);        
		return ETC_FILEIOFAILED;    
	}        
	__mg_rewind (tmp_fp);    
	rc = etc_FileCopy (tmp_fp, etc_fp);
error:    
	fclose (etc_fp);    
	__mg_close_tmpfile (tmp_fp);    
	return rc;
}

