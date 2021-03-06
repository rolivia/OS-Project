#include   <stdio.h>
#include   <string.h>
#include   <stdlib.h>
#include   <libxml/xmlmemory.h>
#include   <libxml/parser.h>

void
parseStory (xmlDocPtr doc, xmlNodePtr cur, xmlChar **path) {
/* first output should be /storyinfo/author/John */		
	xmlNodePtr level;
	xmlChar *path2[1000];
	xmlChar *myfile;
	xmlChar *contents;

	FILE *file = NULL;
	file = fopen("/root/Desktop/Project/fuse-2.6.3/example/path.txt", "a");

	//if it is a file or an empty directory handle ending differently
	if(((cur->xmlChildrenNode->next)==NULL)){
		strcat(path, cur->name);
		contents = xmlNodeListGetString(doc, cur->xmlChildrenNode,1);		
		if((xmlStrcmp(contents, " "))){	
			strcat(path, ":");
		}
		else{
			strcat(path, "/");	
		}	
	}

	else{
		strcat(path, cur->name);
		strcat(path, "/");	
	}
	
	fprintf(file, "%s\n", path);
	fclose(file);
	//printf("%s\n", path);

	/* check and see if there is a child. if not get file, if so continue normally*/

	if(((cur->xmlChildrenNode->next)==NULL)){		
		fopen("/root/Desktop/Project/fuse-2.6.3/example/path.txt", "a");		
		contents = xmlNodeListGetString(doc, cur->xmlChildrenNode,1);
		if((xmlStrcmp(contents, " "))){	
		fprintf(file, "%s\n<<>>\n", contents);	
		} 
		fclose(file);
		//printf("%s\n", path);
	}
	else{
		level = cur->xmlChildrenNode->next;
		while(level!=NULL){
			strcpy(path2, path);	
			parseStory(doc, level, path2);
			level = level->next->next;	
		}
	}
//printf("cur: %s\n", cur->name);
//printf("next cur: %s\n", cur->next->name);

}
static void
parseDoc(char *docname) {
        xmlDocPtr doc;
        xmlNodePtr cur;
	xmlChar *path[1000];
	xmlChar* mylevel;

        doc = xmlParseFile(docname);
        if (doc == NULL ) {
                fprintf(stderr,"Document not parsed successfully. \n");
                return;
        }
        cur = xmlDocGetRootElement(doc);
        if (cur == NULL) {
                fprintf(stderr,"empty document\n");
                xmlFreeDoc(doc);
                return;
        }
        if (xmlStrcmp(cur->name, (const xmlChar *) "root")) {
                fprintf(stderr,"document of the wrong type, root node != root");
                xmlFreeDoc(doc);
                return;
        }
        cur = cur->xmlChildrenNode;
	mylevel = cur->next->name;
	//printf("mylevel: %s\n", mylevel);
        while (cur != NULL) {
                if ((!xmlStrcmp(cur->name, (const xmlChar *)mylevel))){
			//printf("mycurname: %s\n", cur->name);   
			strcpy(path, "/");                     
			 parseStory (doc, cur, path);
			if((cur->next->next)!=NULL){
			strcpy(path, "/");
			mylevel=cur->next->next->name;
                	}	
		}
        cur = cur->next;
        }
        xmlFreeDoc(doc);
        return;
}
int
main(int argc, char **argv) {
        char *docname;
        if (argc <= 1) {
                printf("Usage: %s docname\n", argv[0]);
                return(0);
        }
        docname = argv[1];
        parseDoc (docname);
        return (1);
}
