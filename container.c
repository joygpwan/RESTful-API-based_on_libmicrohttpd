/*
 * Copyright (C) Guangping Wan (joygpwan)
 */

#include "container.h"
#include "request_analysis.h"

#define PORT 8888

int answer_to_connection (void *cls, struct MHD_Connection *connection,const char *url, const char *method,const char *version, const char *upload_data,
                      size_t *upload_data_size, void **con_cls)
{
	struct MHD_Response *response;
	puts(page);
	
	if (strcmp(method,"GET") == 0)
  	{

		printf("%s New %s request for %s using version %s",__func__, method, url, version);
		request_get(cls,  connection, url,  method, version,  upload_data, upload_data_size, con_cls);
		return 1;
  	}    
  	else if (strcmp(method,"PUT") == 0)
  	{
		printf("%s New %s request for %s using version %s",__func__, method, url, version);
		request_put( cls,  connection, url,  method, version,  upload_data, upload_data_size,con_cls);
		return 1;
	}
	else if (strcmp(method,"DELETE") == 0)
	{
		printf("%s New %s request for %s using version %s",__func__, method, url, version);    
		request_delete( cls,  connection, url,  method, version,  upload_data, upload_data_size,con_cls);
  	}
	else if (strcmp(method,"HEAD") == 0)
	{
		printf("%s New %s request for %s using version %s",__func__, method, url, version);
		request_head( cls,  connection, url,  method, version,  upload_data, upload_data_size,con_cls);
		return 1;
  	}

	else if (strcmp(method,"POST") == 0)
	{
		printf("%s New %s request for %s using version %s",__func__, method, url, version);
		request_post(cls,  connection, url,  method, version,  upload_data, upload_data_size, con_cls);
		return 1;
	}
	else
		printf("%s ERROR %s request for %s using version %s",__func__, method, url, version);
		return 0;
}

int
main ()
{
	struct MHD_Daemon *daemon;
	daemon = MHD_start_daemon (MHD_USE_THREAD_PER_CONNECTION, PORT, NULL, NULL,&answer_to_connection, NULL, MHD_OPTION_END);
	if (NULL == daemon)
	{   
		printf("start daemon failed!\n");		
		return 1;
	}
	getchar ();

	MHD_stop_daemon (daemon);
	return 0;
}

