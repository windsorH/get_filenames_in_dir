/*
 * @Author: hfy472309568@gmail.com
 * @Date: 2023-03-21 17:11:09
 * @LastEditors: huangfeiyun
 * @LastEditTime: 2023-03-28 17:37:52
 * @Description: 参考网络资源和代码，递归获取目录下所有特定类型文件的文件名，返回文件个数，不使用 d_type，使用 stat
 * path: 目录路径
 * type: 文件类型
 * withPath: 0:不带路径，1：带路径
 * allfilename: 存储文件名的数组
 * return: 文件个数
 * Copyright (c) 2023 by windsorH/hfy472309568@gmail.com, All Rights Reserved.
 */
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "get_filenames_in_dir.h"

static int get_filename_num = 0;
int get_num_and_names_of_files_in_dir(char *path, char *type, int withPath, char (*allfilename)[GET_FILENAME_MAX_LENGTH])
{
	DIR *dp = NULL;
	struct dirent *st;
	struct stat sta;
	int ret = 0;
	// int get_filename_num = 0;
	char tmp_name[GET_FILENAME_MAX_LENGTH] = {0};
	dp = opendir(path);
	if (dp == NULL)
	{
		// printf("open dir error!!\n");
		return -1;
	}
	while (1)
	{
		st = readdir(dp);
		if (NULL == st) // 读取完毕
		{
			break;
		}
		strcpy(tmp_name, path);
		// if (path[strlen(path) - 1] != '/') // 判断路径名是否带/
		// 	strcat(tmp_name, "/");
		if (path[strlen(path) - 1] != '\\') // 判断路径名是否带/
			strcat(tmp_name, "\\");
		strcat(tmp_name, st->d_name); // 新文件路径名
		ret = stat(tmp_name, &sta);	  // 查看目录下文件属性
		if (ret < 0)
		{
			// printf("read stat fail\n");
			return -1;
		}

		if (S_ISDIR(sta.st_mode)) // 如果为目录文件
		{
			if (0 == strcmp("..", st->d_name) || 0 == strcmp(".", st->d_name)) // 如果为.或..则跳过
			{
				continue;
			}
			else
			{
				// 递归读取
				get_filename_num = get_num_and_names_of_files_in_dir(tmp_name, type, withPath, allfilename);
			}
		}
		else // 不为目录则打印文件路径名
		{
			if (withPath > 0)
			{
				// printf("%s, %d\n", tmp_name,get_filename_num);
				for (int i = 0; i < GET_FILENAME_MAX_LENGTH; i++)
				{
					allfilename[get_filename_num][i] = tmp_name[i];
				}
				get_filename_num++;
			}
			else
			{
				// printf("%s, %d\n", st->d_name, get_filename_num);
				for (int i = 0; i < GET_FILENAME_MAX_LENGTH; i++)
				{
					allfilename[get_filename_num][i] = st->d_name[i];
				}
				get_filename_num++;
			}
		}
	}
	closedir(dp);
	return get_filename_num;
}

int get_personal_info_from_files(char *filename, float *data)
{
	// 从文件名获取个人信息
	// 例如文件名 3axis_N15-VO2MAX-D2023028-BL-niehailong-5-LS-left-S1A25W65H178-VA43.3K42.4-offLine.csv

	// S 后的数字为性别 sex = 1:男，sex = 0:女
	// A 后的数字为年龄 age = 25
	// W 后的数字为体重	weight = 65
	// H 后的数字为身高	height = 178
	// VA 后的数字为 apple watch VO2MAX
	// VK 后的数字为 Keep VO2MAX
	// 通过 - 分割文件名字符串 filename
	char *temp;
	char *p = strtok_s(filename, "-", &temp);
	int i = 0;
	while (p != NULL)
	{
		p = strtok_s(NULL, "-", &temp);
		i++;
		// -S 终止分割
		if (i == 7)
		{
			break;
		}
	}
	// 分别读取性别、年龄、身高和体重 存入 data
	float sum = 0; // data[4];
	int top = 0;
	i = 0;
	int decimal_places = 0;
	while (temp[i] != '\0')
	{
		if (sum == 0 && p[i] == '0')
		{
			data[top++] = sum;
			sum = 0;
		}
		if (p[i] == '.')
		{
			decimal_places++;
		}
		else if (p[i] >= '0' && p[i] <= '9')
		{
			sum = sum * 10 + (p[i] - '0'); // 连续数字转换为数.
		}
		else
		{
			if (sum != 0) // if (sum != 0)//保存这个数
			{
				if (decimal_places > 0)
				{
					sum = sum / (pow(10, decimal_places));
				}
				data[top++] = sum;
				sum = 0;
				decimal_places = 0;
			}
		}
		i++;
	}
	if (sum != 0) // 判断最后一个字符是否是数
	{
		data[top++] = sum;
	}
	// i = 0;
	return 0;
}