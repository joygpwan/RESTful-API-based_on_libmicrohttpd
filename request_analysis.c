
/*
 * Copyright (C) Guangping Wan (joygpwan)
 */

#include "request_analysis.h"
#include "container.h"
#include "object_response_header.h"
#include "xml.h"
#include "bucket_response_header.h"


int
request_get(void *cls, struct MHD_Connection *connection,const char *url, const char *method,const char *version,  \ 
	const char *upload_data,size_t *upload_data_size, void **con_cls)
{
	char pathname[128];
	//char bucket[64];
	char b_host[128];
	char *temp;
	char *pathname_xml;
	char *delim=".";
	int n,m,i,ret,fd;
	struct stat sbuf;;
	struct MHD_Response *response;
	int count[1];
	char buftest[BUFTESTSIZE];
	sender_head send_header;
	
	temp=MHD_lookup_connection_value(connection, MHD_HEADER_KIND, "Host");
	printf("request_analysis 81 line host is %s\n",temp);
	strncpy(b_host,temp,strlen(temp)+1);
	strtok(b_host, delim);
	strncpy(pathname,b_host,strlen(b_host)+1);
	//strncpy(pathname_xml,b_host,strlen(b_host)+1);
	//strcat(pathname_xml,".xml");
	pathname_xml=my_itoa(getpid());
	
	//printf("url is %s\n",url);

	strncpy(send_header.Connection,"close",strlen("close")+1);
	printf("request_analysis.c 95line  date is %s \n",send_header.Connection);

	strncpy(send_header.Pathname,url,strlen(url)+1);
	printf("request_analysis.c 98 line  url is %s \n",send_header.Pathname);
	//***************************sender_head end

	if(is_dir_xml(url)){
		printf("%s List the Contents of  bucket\n: %s",__func__, url);
		creat_xml(pathname_xml);
		printf("\n pathname : %s\n",pathname);
	
		//is_dir_xml(const char *path)
		delete_file_xml(url,count,pathname_xml);

		//******************test http header********************** 

		temp=MHD_lookup_connection_value(connection, MHD_HEADER_KIND, "Date");
		if(NULL == temp)
			printf("GET GET request_analysis.c 116 line send package date is NULL\n");
		else
			printf("GET GET request_analysis.c 172 line send package date: %s \n",temp);

	
		//*********************end test http header**************
	
		if ( (-1 == (fd = open (pathname_xml, O_RDONLY))) ||(0 != fstat (fd, &sbuf)) )
		{
			/* error accessing file */
			if (fd != -1) 
				close (fd);
			const char *errorstr ="<html><body>error accessing bucket has occured!</body></html>";
			response =MHD_create_response_from_buffer (strlen (errorstr),(void *) errorstr,MHD_RESPMEM_PERSISTENT);
			if (response)
			{
				get_bucket_header_no(response,send_header);
				ret =MHD_queue_response (connection, MHD_HTTP_INTERNAL_SERVER_ERROR,response);
				MHD_destroy_response (response);
				return MHD_YES;
			}
			else
				return MHD_NO;
		}
		response =MHD_create_response_from_fd_at_offset (sbuf.st_size, fd, 0);
		if(response)
		{
			//MHD_add_response_header (response, "Content-Type", MIMETYPE);
			get_bucket_header_yes(response,send_header);
			ret = MHD_queue_response (connection, MHD_HTTP_OK, response);

			MHD_destroy_response (response);
			return MHD_YES;
		}
		else
			return MHD_NO;

	}
	//***************************************get object************	
	else 
	{
		printf("%s Get object: %s\n", __func__,url);
		printf("PATHNAME is %s,URL is %s\n",pathname,url);
		
		if ( (-1 == (fd = open (url, O_RDONLY))) ||(0 != fstat (fd, &sbuf)) )
		{
			if (fd != -1) 
				close (fd);
			const char *errorstr ="<html><body>error accessing file has occured!<br>access the error file!</body></html>";
			response =MHD_create_response_from_buffer (strlen(errorstr),(void *)errorstr,MHD_RESPMEM_PERSISTENT);
			if (response)
			{
				get_object_header_no(response,send_header);
				ret =MHD_queue_response (connection, MHD_HTTP_INTERNAL_SERVER_ERROR,response);
				MHD_destroy_response (response);
				return MHD_YES;
			}
		else
			return MHD_NO;
		}

		response =MHD_create_response_from_fd_at_offset (sbuf.st_size, fd, 0);
		if(response)
		{
			get_object_header_yes(response,send_header);
			ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
			MHD_destroy_response (response);
			return MHD_YES;
		}
		else
			return MHD_NO;
	}
	return 1;
}


int
request_put(void *cls, struct MHD_Connection *connection,         \
                      const char *url, const char *method,         \
                      const char *version,const char *upload_data,   \
                      size_t *upload_data_size, void **con_cls)
{
  
	char pathname[128];
	char bucket[64];
	char b_host[128];
	char *temp;
	char *pathname_xml;
	char *delim=".";
	int n,m,i,ret,fd;
	struct stat sbuf;;
	struct MHD_Response *response;
	int count[1];
	char buftest[BUFTESTSIZE];
	sender_head send_header;
	char *bucket_object[128];
	
	
	temp=MHD_lookup_connection_value(connection, MHD_HEADER_KIND, "Host");
	printf("request_analysis 934 line host is %s\n",temp);
	strncpy(b_host,temp,strlen(temp)+1);
	strtok(b_host, delim);
	strncpy(pathname,b_host,strlen(b_host)+1);

	strncpy(send_header.Connection,"close",strlen("close")+1);
	printf("request_analysis.c 95line  date is %s \n",send_header.Connection);

	strncpy(send_header.Pathname,url,strlen(url)+1);
	printf("request_analysis.c 98 line  url is %s \n",send_header.Pathname);
	
	get_sonstr(url,bucket_object);
	if(judge_object_or_bucket_in_put(bucket_object)>0)
	{

		printf("POST a bucket named %s\n",bucket_object);
		if(mkdir(url,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)<0)
		{		
			printf("create bucket %s error !\n",bucket_object);
			return 0;
		}
		response =MHD_create_response_from_buffer(0,NULL, MHD_RESPMEM_PERSISTENT);
		if(response)
		{
					
			post_bucket_header_yes(response,send_header);	
			ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
			MHD_destroy_response (response);
			return MHD_YES;
		}
		else
			return MHD_NO;

	}		
	else{
		printf("%s Put  data: %s",__func__, url);
		printf(" PUT :  lookup content-type before  set:   %s  \n " ,MHD_lookup_connection_value(connection,MHD_HEADER_KIND,MHD_HTTP_HEADER_CONTENT_TYPE) );

		if (NULL == *con_cls)
    	{
      			struct connection_info_struct *con_info;
				if (nr_of_uploading_clients >= MAXCLIENTS)
					return send_page (connection, busypage, MHD_HTTP_SERVICE_UNAVAILABLE);
				
				con_info = malloc (sizeof (struct connection_info_struct));
      			if (NULL == con_info)
        			return MHD_NO;

				con_info->fp = NULL;

				if(MHD_set_connection_value(connection,MHD_HEADER_KIND,MHD_HTTP_HEADER_CONTENT_TYPE, MHD_HTTP_POST_ENCODING_MULTIPART_FORMDATA)==MHD_YES)
					printf("enctype= url  successs!\n");
				printf(" now  lookup enctype   %s  \n " ,MHD_lookup_connection_value(connection,MHD_HEADER_KIND,MHD_HTTP_HEADER_CONTENT_TYPE) );

				if(MHD_set_connection_value(connection,MHD_HEADER_KIND,"type","file")==MHD_YES)
				{
					if (0 == strcmp (method, "PUT"))
					{
						con_info->postprocessor =MHD_create_post_processor (connection, POSTBUFFERSIZE,iterate_post, (void *) con_info);
						if (NULL == con_info->postprocessor)
						{
							free (con_info);
							printf("MHD_create_post_processor error!\n");
							return MHD_NO;
						}

						nr_of_uploading_clients++;
						con_info->connectiontype = POST;
						con_info->answercode = MHD_HTTP_OK;
						con_info->answerstring = completepage;
					}
					else
        			
        				con_info->connectiontype = GET;
					con_cls = (void *) con_info;
					return MHD_YES;
				}
				
				if (0 == strcmp (method, "GET"))
    			{
      				char buffer[1024];
					snprintf (buffer, sizeof (buffer), askpage, nr_of_uploading_clients);
					return send_page (connection, buffer, MHD_HTTP_OK);
				}

				if (0 == strcmp (method, "PUT"))
				{
					struct connection_info_struct *con_info = *con_cls;
					printf("before  MHD_post_process  upload_data_size is %d \n",*upload_data_size);
					if (0 != *upload_data_size)
					{
						printf("now is  MHD_post_process  ,before  \n");
						printf(" now  before MHD_post_process  enctype    %s  \n " ,MHD_lookup_connection_value(connection,MHD_HEADER_KIND,MHD_HTTP_HEADER_CONTENT_TYPE) );
						if( MHD_post_process (con_info->postprocessor, upload_data,*upload_data_size)==MHD_YES)
							printf("MHD_post_process sucess \n");

						*upload_data_size = 0;
						return MHD_YES;
					}
					else
					{
						if (NULL != con_info->fp)
						{
							fclose (con_info->fp);
							con_info->fp = NULL; 
						}
	 
						return send_page (connection, con_info->answerstring,con_info->answercode);
					}

					//  return send_page (connection, con_info->answerstring,con_info->answercode);
					return send_page (connection, errorpage, MHD_HTTP_BAD_REQUEST);
				}
		}
		return MHD_NO;
	}
}

int
request_delete(void *cls, struct MHD_Connection *connection,         \
                      const char *url, const char *method,         \
                      const char *version,const char *upload_data,   \
                      size_t *upload_data_size, void **con_cls)
{

	char pathname[128];
	//char bucket[64];
	char b_host[128];
	char *temp;
	char *pathname_xml;
	char *delim=".";
	int n,m,i,ret,fd;
	struct stat sbuf;;
	struct MHD_Response *response;
	int count[1];
	char buftest[BUFTESTSIZE];
	sender_head send_header;
	
	temp=MHD_lookup_connection_value(connection, MHD_HEADER_KIND, "Host");
	printf("request_analysis 81 line host is %s\n",temp);
	strncpy(b_host,temp,strlen(temp)+1);
	strtok(b_host, delim);
	strncpy(pathname,b_host,strlen(b_host)+1);

	pathname_xml=my_itoa(getpid());
	

	strncpy(send_header.Connection,"close",strlen("close")+1);
	printf("request_analysis.c 95line  date is %s \n",send_header.Connection);

	strncpy(send_header.Pathname,url,strlen(url)+1);
	printf("request_analysis.c 98 line  url is %s \n",send_header.Pathname);
	//***************************sender_head end

	if(is_dir_xml(url)){

		delete_file(url);
		delete_dir(url);
	
		response =MHD_create_response_from_buffer(0,NULL, MHD_RESPMEM_PERSISTENT);
		if(response)
		{
			//MHD_add_response_header (response, "Content-Type", MIMETYPE);
			delete_bucket_header_yes(response,send_header);
			ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
			MHD_destroy_response (response);
			return MHD_YES;
		}
		else
			return MHD_NO;

	}
	else
	{
		if(unlink(url)<0)
		{
			perror("490 line DELETE request_delete unlink \n");
			return 0;
		}


		response =MHD_create_response_from_buffer(0,NULL, MHD_RESPMEM_PERSISTENT);
		if(response)
		{
			//MHD_add_response_header (response, "Content-Type", MIMETYPE);
			delete_object_header_yes(response,send_header);

			ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
			MHD_destroy_response (response);
			return MHD_YES;
		}
		else
			return MHD_NO;

	}
	return 1;
}


int
request_head(void *cls, struct MHD_Connection *connection,         \
                      const char *url, const char *method,         \
                      const char *version,const char *upload_data,   \
                      size_t *upload_data_size, void **con_cls)
{

	char pathname[128];
	//char bucket[64];
	char b_host[128];
	char *temp;
	char *pathname_xml;
	char *delim=".";
	int n,m,i,ret,fd;
	DIR *dp;
	struct stat sbuf;;
	struct MHD_Response *response;
	int count[1];
	char buftest[BUFTESTSIZE];
	sender_head send_header;
	
	
	temp=MHD_lookup_connection_value(connection, MHD_HEADER_KIND, "Host");
	printf("645 line HEAD HEAD request_analysis 81 line host is %s\n",temp);
	strncpy(b_host,temp,strlen(temp)+1);
	strtok(b_host, delim);
	strncpy(pathname,b_host,strlen(b_host)+1);


	//************************sender_head 

	strncpy(send_header.Connection,"close",strlen("close")+1);
	printf("HEAD HEAD request_analysis.c 659 line  date is %s \n",send_header.Connection);

	strncpy(send_header.Pathname,url,strlen(url)+1);
	printf("HEAD HEAD request_analysis.c 662 line  url is %s \n",send_header.Pathname);
	//***************************sender_head end

	if(is_dir_xml(url)){
		printf("%s HEAD HEAD the attribute of  bucket\n: %s",__func__, url);
		// HEAD  creat_xml(pathname_xml);
		printf("\n pathname : %s\n",pathname);
	

		temp=MHD_lookup_connection_value(connection, MHD_HEADER_KIND, "Date");
		if(NULL == temp)
			printf("HEAD HEAD request_analysis.c 681 line  send package date is NULL\n");
		else
			printf("HEAD HEAD request_analysis.c 683 line get response header date: %s \n",temp);

		if (NULL== (dp= opendir (url)))
		{
			const char *errorstr ="<html><body>HEAD The bucket not existed!</body></html>";
			response =MHD_create_response_from_buffer (strlen (errorstr),(void *) errorstr,MHD_RESPMEM_PERSISTENT);
		
			if(closedir(dp)<0)
				printf("IN HEAD close dir error!\n");		

			if (response)
			{

				head_bucket_header_no(response,send_header);
				ret =MHD_queue_response (connection, MHD_HTTP_INTERNAL_SERVER_ERROR,response);
				MHD_destroy_response (response);
				return MHD_YES;
			}
		
			else
				return MHD_NO;
		}
		else
		{
			response =MHD_create_response_from_buffer(0,NULL, MHD_RESPMEM_PERSISTENT);
			if(response)
			{
				//MHD_add_response_header (response, "Content-Type", MIMETYPE);
				head_bucket_header_yes(response,send_header);
				ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
				MHD_destroy_response (response);
				return MHD_YES;
			}
			else
				return MHD_NO;

		}		
	
	}
  	else 
	{

		printf("%s HEAD HEAD object: %s\n", __func__,url);
		if (0!= fstat (fd, &sbuf))
		{
			const char *errorstr ="<html><body>error accessing file has occured!<br>access the error file!</body></html>";
			response =MHD_create_response_from_buffer (strlen(errorstr),(void *)errorstr,MHD_RESPMEM_PERSISTENT);
			if (response)
			{
				head_object_header_no(response,send_header);
				ret =MHD_queue_response (connection, MHD_HTTP_INTERNAL_SERVER_ERROR,response);
				
				MHD_destroy_response (response);
				return MHD_YES;
			}
		
		else
			return MHD_NO;
		}

		response =MHD_create_response_from_buffer(0,NULL, MHD_RESPMEM_PERSISTENT);
		if(response)
		{
			//MHD_add_response_header (response, "Content-Type", MIMETYPE);
			head_object_header_yes(response,send_header);
			ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
			MHD_destroy_response (response);
			return MHD_YES;
		}
		else
			return MHD_NO;

	}
	return 1;
}



int
request_post(void *cls, struct MHD_Connection *connection,         \
                      const char *url, const char *method,         \
                      const char *version,const char *upload_data,   \
                      size_t *upload_data_size, void **con_cls)
{
  
	char pathname[128];
	char bucket[64];
	char b_host[128];
	char *temp;
	char *pathname_xml;
	char *delim=".";
	int n,m,i,ret,fd;
	struct stat sbuf;;
	struct MHD_Response *response;
	int count[1];
	char buftest[BUFTESTSIZE];
	sender_head send_header;
	//*****************add to generate a bucket or object name
	char *bucket_object[128];
	
	temp=MHD_lookup_connection_value(connection, MHD_HEADER_KIND, "Host");
	printf("request_analysis 934 line host is %s\n",temp);
	strncpy(b_host,temp,strlen(temp)+1);
	strtok(b_host, delim);
	strncpy(pathname,b_host,strlen(b_host)+1);


	strncpy(send_header.Connection,"close",strlen("close")+1);
	printf("request_analysis.c 95line  date is %s \n",send_header.Connection);

	strncpy(send_header.Pathname,url,strlen(url)+1);
	printf("request_analysis.c 98 line  url is %s \n",send_header.Pathname);


	get_sonstr(url,bucket_object);
	if(judge_object_or_bucket_in_put(bucket_object)>0)
	{
		//***********************POST bucket***********************
		printf("POST a bucket named %s\n",bucket_object);
		if(mkdir(url,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)<0)
		{		
			printf("create bucket %s error !\n",bucket_object);
			return 0;
		}
		response =MHD_create_response_from_buffer(0,NULL, MHD_RESPMEM_PERSISTENT);
		if(response)
		{
					
			post_bucket_header_yes(response,send_header);	
			ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
			MHD_destroy_response (response);
			return MHD_YES;
		}
		else
			return MHD_NO;

	}		
	
	else{
		printf("%s Post  data: %s",__func__, url);
		printf(" POST :  lookup content-type before  set:   %s  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n " ,MHD_lookup_connection_value(connection,MHD_HEADER_KIND,MHD_HTTP_HEADER_CONTENT_TYPE) );


		if (NULL == *con_cls)
		{
			struct connection_info_struct *con_info;

			if (nr_of_uploading_clients >= MAXCLIENTS)
				return send_page (connection, busypage, MHD_HTTP_SERVICE_UNAVAILABLE);

			con_info = malloc (sizeof (struct connection_info_struct));
			if (NULL == con_info)
				return MHD_NO;

			con_info->fp = NULL;

			if (0 == strcmp (method, "POST"))
			{
				con_info->postprocessor =MHD_create_post_processor (connection, POSTBUFFERSIZE,iterate_post, (void *) con_info);
				if (NULL == con_info->postprocessor)
				{
					free (con_info);
					printf("MHD_create_post_processor error!\n");
					return MHD_NO;
				}
				
          			nr_of_uploading_clients++;
				con_info->connectiontype = POST;
				con_info->answercode = MHD_HTTP_OK;
				con_info->answerstring = completepage;
			}
			else
				con_info->connectiontype = GET;

			*con_cls = (void *) con_info;
			return MHD_YES;
		}

		if (0 == strcmp (method, "GET"))
		{
			char buffer[1024];
			snprintf (buffer, sizeof (buffer), askpage, nr_of_uploading_clients);
			return send_page (connection, buffer, MHD_HTTP_OK);
		}

		if (0 == strcmp (method, "POST"))
		{
			struct connection_info_struct *con_info = *con_cls;
			if (0 != *upload_data_size)
			{
				printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!before  MHD_post_process  upload_data_size is %d \n",*upload_data_size);
          			if(MHD_post_process (con_info->postprocessor, upload_data,*upload_data_size)==MHD_YES)
              			printf("MHD_PROcess_post  sucess !!!\n");
          			*upload_data_size = 0;

          			return MHD_YES;
			}
			else
			{
				if (NULL != con_info->fp)
				{
					fclose (con_info->fp);
					con_info->fp = NULL; 

				}
	  			/* Now it is safe to open and inspect the file before calling send_page with a response */
	  			return send_page (connection, con_info->answerstring,con_info->answercode);
	  		}
		// return send_page (connection, con_info->answerstring,con_info->answercode);
			return send_page (connection, errorpage, MHD_HTTP_BAD_REQUEST);
		}
	}
	printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@last!\n");
	//  return send_page (connection, errorpage, MHD_HTTP_BAD_REQUEST);
	return MHD_NO;
}



