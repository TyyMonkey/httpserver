#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "myhttp.h"

/**
*
*@ret:SUCCESS 0 or length of filename, ERROR -1
*/
int myhttp_get_filename(char *msg, char **filename)
{
	if(msg == NULL)
	{
		printf("msg is NULL");
		return -1;
	}
	int ret = 0;
	int start = 0;
	int end = 0;
	int i = 0;
	for(i = 0; i < strlen(msg); i++)
	{
		if(msg[i] == ' ')
		{
			if(0 == start)
			{
				start = i + 2;
			}else if(start != 0 && 0 == end)
			{
				end = i;
				break;
			}
		}
	}

	int len = end - start;
	if(len >= 0)
	{
		*filename = (char *)malloc((len + 1) * sizeof(char) );
		
		if(*filename == NULL)
		{
			printf("malloc failed %s\n", strerror(errno));
			ret = -1;
			goto end;
		}
			
		for(i = 0; i < len; i++)
		{
			(*filename)[i] = msg[start + i];
		}
		(*filename)[len] = '\0';
		ret = len;
	}
	else
	{
		ret = -1;
		*filename = NULL;
	}
	
end:	return ret;
}

/*
ai           application/postscript              
aif          audio/x-aiff                        
aifc         audio/x-aiff                        
aiff         audio/x-aiff                        
asc          text/plain                          
au           audio/basic                        
avi          video/x-msvideo                    
bcpio        application/x-bcpio                
bin          application/octet-stream            
bmp          image/bmp                          
cdf          application/x-netcdf                
class        application/octet-stream            
cpio         application/x-cpio                  
cpt          application/mac-compactpro          
csh          application/x-csh                  
css          text/css                            
dcr          application/x-director              
dir          application/x-director              
djv          image/vnd.djvu                      
djvu         image/vnd.djvu                      
dll          application/octet-stream            
dms          application/octet-stream            
doc          application/msword                  
dvi          application/x-dvi                  
dxr          application/x-director              
eps          application/postscript              
etx          text/x-setext                      
exe          application/octet-stream            
ez           application/andrew-inset            
gif          image/gif                          
gtar         application/x-gtar                  
hdf          application/x-hdf                  
hqx          application/mac-binhex40            
htm          text/html                          
html         text/html                          
ice          x-conference/x-cooltalk            
ief          image/ief                          
iges         model/iges                          
igs          model/iges                          
jpe          image/jpeg                          
jpeg         image/jpeg                          
jpg          image/jpeg                          
js           application/x-javascript            
kar          audio/midi                          
latex        application/x-latex                
lha          application/octet-stream            
lzh          application/octet-stream            
m3u          audio/x-mpegurl                    
man          application/x-troff-man            
me           application/x-troff-me              
mesh         model/mesh                          
mid          audio/midi                          
midi         audio/midi                          
mif          application/vnd.mif                
mov          video/quicktime                    
movie        video/x-sgi-movie                  
mp2          audio/mpeg                          
mp3          audio/mpeg                          
mpe          video/mpeg                          
mpeg         video/mpeg                          
mpg          video/mpeg                          
mpga         audio/mpeg                          
ms           application/x-troff-ms              
msh          model/mesh                          
mxu          video/vnd.mpegurl                  
nc           application/x-netcdf                
oda          application/oda                    
pbm          image/x-portable-bitmap            
pdb          chemical/x-pdb                      
pdf          application/pdf                    
pgm          image/x-portable-graymap            
pgn          application/x-chess-pgn            
png          image/png                          
pnm          image/x-portable-anymap            
ppm          image/x-portable-pixmap            
ppt          application/vnd.ms-powerpoint      
ps           application/postscript              
qt           video/quicktime                    
ra           audio/x-realaudio                  
ram          audio/x-pn-realaudio                
ras          image/x-cmu-raster                  
rgb          image/x-rgb                        
rm           audio/x-pn-realaudio                
roff         application/x-troff                
rpm          audio/x-pn-realaudio-plugin        
rtf          text/rtf                            
rtx          text/richtext                      
sgm          text/sgml                          
sgml         text/sgml                          
sh           application/x-sh                    
shar         application/x-shar                  
silo         model/mesh                          
sit          application/x-stuffit              
skd          application/x-koan                  
skm          application/x-koan                  
skp          application/x-koan                  
skt          application/x-koan                  
smi          application/smil                    
smil         application/smil                    
snd          audio/basic                        
so           application/octet-stream            
spl          application/x-futuresplash          
src          application/x-wais-source          
sv4cpio      application/x-sv4cpio              
sv4crc       application/x-sv4crc                
swf          application/x-shockwave-flash      
t            application/x-troff                
tar          application/x-tar                  
tcl          application/x-tcl                  
tex          application/x-tex                  
texi         application/x-texinfo              
texinfo      application/x-texinfo              
tif          image/tiff                          
tiff         image/tiff                          
tr           application/x-troff                
tsv          text/tab-separated-values          
txt          text/plain                          
ustar        application/x-ustar                
vcd          application/x-cdlink                
vrml         model/vrml                          
wav          audio/x-wav                        
wbmp         image/vnd.wap.wbmp                  
wbxml        application/vnd.wap.wbxml          
wml          text/vnd.wap.wml                    
wmlc         application/vnd.wap.wmlc            
wmls         text/vnd.wap.wmlscript              
wmlsc        application/vnd.wap.wmlscriptc      
wrl          model/vrml                          
xbm          image/x-xbitmap                    
xht          application/xhtml+xml              
xhtml        application/xhtml+xml              
xls          application/vnd.ms-excel            
xml          text/xml                            
xpm          image/x-xpixmap                    
xsl          text/xml                            
xwd          image/x-xwindowdump                
xyz          chemical/x-xyz                      
zip          application/zip
*/
char * myhttp_gettype(const char *filename)
{	
	if(filename == NULL)
		return "text/html";
		
	//获取文件类型
	char sExt[20] = {0};
	//获取文件类型
		
	int i = 0;
	int start = 0;
	for(i = 0; i < strlen(filename); i++)
	{
		if('.' == filename[i])
		{
			start = i+1;
		}
	}
	strcpy(sExt, filename + start);


  if (strncmp(sExt, "bmp", 3) == 0)
        return "image/bmp";  
  
  if (strncmp(sExt, "gif", 3) == 0)  
      return "image/gif";  

  if (strncmp(sExt, "ico", 3) == 0)  
      return "image/x-icon";  

  if (strncmp(sExt, "jpg", 3) == 0)
      return "image/jpeg";  

  if (strncmp(sExt, "avi", 3) == 0)  
      return "video/avi";  

  if (strncmp(sExt, "css", 3) == 0)  
      return "text/css";  

  if (strncmp(sExt, "dll", 3) == 0)  
      return "application/x-msdownload";  

  if (strncmp(sExt, "exe", 3) == 0)  
      return "application/x-msdownload";  

  if (strncmp(sExt, "dtd", 3) == 0)  
      return "text/xml";  

  if (strncmp(sExt, "mp3", 3) == 0)  
      return "audio/mp3";  

  if (strncmp(sExt, "mpg", 3) == 0)  
      return "video/mpg";  

  if (strncmp(sExt, "png", 3) == 0)  
      return "image/png";  

  if (strncmp(sExt, "ppt", 3) == 0)  
      return "application/vnd.ms-powerpoint";  

  if (strncmp(sExt, "xls", 3) == 0)  
      return "application/vnd.ms-excel";  

  if (strncmp(sExt, "doc", 3) == 0)  
      return "application/msword";  

  if (strncmp(sExt, "mp4", 3) == 0)  
      return "video/mpeg4";  

  if (strncmp(sExt, "ppt", 3) == 0)  
      return "application/x-ppt";  

  if (strncmp(sExt, "wma", 3) == 0)  
      return "audio/x-ms-wma";  

  if (strncmp(sExt, "wmv", 3) == 0)  
      return "video/x-ms-wmv";  
      
	return "text/html";
}