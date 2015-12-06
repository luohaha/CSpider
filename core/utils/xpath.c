#include "utils.h"

/*
  xml : 待解析的文件
  path : 解析该文件的xpath法制
*/
int xpath(char *xml, char *path, char **get, int num) { 

  int size;
    /* Init libxml */     
    xmlInitParser();
    //LIBXML_TEST_VERSION

    /* Do the main job */
    size = execute_xpath_expression(xml, BAD_CAST path, get, num);
      if(size == -1) {
	
	return -1;
      }

    /* Shutdown libxml */
    xmlCleanupParser();
    
    /*
     * this is to debug memory for regression tests
     */
    //xmlMemoryDump();
    return size;
}


/**
 * execute_xpath_expression:
 * @filename:		the input XML filename.
 * @xpathExpr:		the xpath expression for evaluation.
 * @nsList:		the optional list of known namespaces in 
 *			"<prefix1>=<href1> <prefix2>=href2> ..." format.
 *
 * Parses input XML file, evaluates XPath expression and prints results.
 *
 * Returns 0 on success and a negative value otherwise.
 */
int execute_xpath_expression(const char* filename, const xmlChar* xpathExpr, char **get, int num) {
    xmlDocPtr doc;
    xmlXPathContextPtr xpathCtx; 
    xmlXPathObjectPtr xpathObj; 
    
    assert(filename);
    assert(xpathExpr);

    /* Load XML document */
    //doc = xmlParseFile(filename);
    
    doc = htmlReadMemory(filename, strlen(filename), NULL, NULL,  HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
    //doc = htmlDocPtr(filename, NULL);
    if (doc == NULL) {
	fprintf(stderr, "Error: unable to parse this string\n");
	return(-1);
    }
    
    /* Create xpath evaluation context */
    xpathCtx = xmlXPathNewContext(doc);
    if(xpathCtx == NULL) {
        fprintf(stderr,"Error: unable to create new XPath context\n");
        xmlFreeDoc(doc); 
        return -1;
    }
    

    /* Evaluate xpath expression */
    xpathObj = xmlXPathEvalExpression(xpathExpr, xpathCtx);
    if(xpathObj == NULL) {
        fprintf(stderr,"Error: unable to evaluate xpath expression \"%s\"\n", xpathExpr);
        xmlXPathFreeContext(xpathCtx); 
        xmlFreeDoc(doc); 
        return -1;
    }

    /* Print results */
    int size = print_xpath_nodes(xpathObj->nodesetval, get, num);

    /* Cleanup */
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx); 
    xmlFreeDoc(doc); 
    
    return size;
}


/**
 * print_xpath_nodes:
 * @nodes:		the nodes set.
 * @output:		the output file handle.
 *
 * Prints the @nodes content to @output.
 */
int print_xpath_nodes(xmlNodeSetPtr nodes, char **get, int num) {
    xmlNodePtr cur;
    int size;
    int i;
    
    //assert(output);
    size = (nodes) ? nodes->nodeNr : 0;
    
    //fprintf(output, "Result (%d nodes):\n", size);
    for(i = 0; i < size && i < num; ++i) {
        get[i] = (char*)malloc(sizeof(char));
	assert(nodes->nodeTab[i]);
	cur = (xmlNodePtr)nodes->nodeTab[i];
	//printf("key : %s \n value : %s\n", cur->name, xmlNodeGetContent(cur));
	get[i] = xmlNodeGetContent(cur);
    }

    return (size<num)?size:num;
}

/*
int main() {
   char *xml = "<doc href=\"jb\"><ni>one</ni><ni>two</ni></doc>";
   char *path = "/doc/ni";
   char *get[10];
   int i;
   int size = xpath(xml, path, get);
   for (i = 0; i < size; i++)
     printf("%s\n", get[i]);
   return 0;
 }
*/
