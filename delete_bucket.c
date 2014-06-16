/*
 * Copyright (C) Guangping Wan (joygpwan)
 */
 
#include "delete_bucket.h"


bool is_dir(const char *path)
{
	struct stat statbuf;
	if(lstat(path, &statbuf) ==0)//lstat�����ļ�����Ϣ���ļ���Ϣ������stat�ṹ��
	{
		return S_ISDIR(statbuf.st_mode) != 0;//S_ISDIR�꣬�ж��ļ������Ƿ�ΪĿ¼
	}
	return false;
}
	
	//�ж��Ƿ�Ϊ�����ļ�
bool is_file(const char *path)
{
	struct stat statbuf;
	if(lstat(path, &statbuf) ==0)
		return S_ISREG(statbuf.st_mode) != 0;//�ж��ļ��Ƿ�Ϊ�����ļ�
	return false;
}
	
	//�ж��Ƿ�������Ŀ¼
bool is_special_dir(const char *path)
{
	return strcmp(path, ".") == 0 || strcmp(path, "..") == 0;
}
	
	//�����������ļ�·��
void get_file_path(const char *path, const char *file_name,  char *file_path)
{
	strcpy(file_path, path);
	if(file_path[strlen(path) - 1] != '/')
		strcat(file_path, "/");
	strcat(file_path, file_name);
}
	
void delete_file(const char *path)
{
	DIR *dir;
	struct dirent *dir_info;
	char file_path[PATH_MAX];
	if(is_file(path))
	{
		remove(path);
		return;
	}
	if(is_dir(path))
	{
		if((dir = opendir(path)) == NULL)
			return;
		while((dir_info = readdir(dir)) != NULL)
		{
			get_file_path(path, dir_info->d_name, file_path);
			if(is_special_dir(dir_info->d_name))
				continue;
			delete_file(file_path);
			rmdir(file_path);
		}
	}
	
}

int delete_dir(const char *path)
{
	if(rmdir(path)==0)
	{
		printf("Delete success!\n");
			return 1;
	}
	else 
	{
		printf("\n Delete failed\n");
   		return 0;
	}
		
}

