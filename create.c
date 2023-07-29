#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "ezxml/ezxml.h"

char *rs(char *s) {
	char *p=s;
	while(*p) {
		if(*p==' ') *p='_';
		p++;
	}
	return s;
}

int main(void) {

	ezxml_t bible = ezxml_parse_file("kjv.xml"),book,chap,vers;

	const char *bname,*text;
	int bnum,cnum,vnum;
	struct stat st = {0};
	char folder[256]="";
	char filename[256]="";

	if (stat("kjv", &st) == -1) {
	    mkdir("kjv", 0700);
	}

	for (book = ezxml_child(bible, "BIBLEBOOK"); book; book = book->next) {

		bname = ezxml_attr(book, "bname");
		bnum = atoi(ezxml_attr(book, "bnumber"));

		sprintf(folder,"kjv/%02d_%s",bnum,bname);

		rs(folder);

		if (stat(folder, &st) == -1) {
			mkdir(folder, 0700);
		}
		
		for (chap = ezxml_child(book, "CHAPTER"); chap; chap = chap->next) {

			cnum = atoi(ezxml_attr(chap, "cnumber"));

			sprintf(filename,"%s/%s_%03d.gmi",folder,bname,cnum);

			FILE *fp=fopen(filename,"w");

			for (vers = ezxml_child(chap, "VERS"); vers; vers = vers->next) {
				vnum = atoi(ezxml_attr(vers, "vnumber"));
				text = ezxml_txt(vers);
				fprintf(fp,"%s %d:%d -> %s\n\n",bname,cnum,vnum,text);
			}
			
			fclose(fp);
		}
	}

	ezxml_free(bible);

	return 0;
}
