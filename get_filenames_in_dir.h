/*
 * @Author: hfy472309568@gmail.com
 * @Date: 2023-03-21 17:11:09
 * @LastEditors: windsorhuang
 * @LastEditTime: 2023-03-27 16:27:25
 * @Description: 参考网络资源和代码，递归获取目录下所有特定类型文件的文件名，返回文件个数，不使用 d_type，使用 stat
 * path: 目录路径
 * type: 文件类型
 * withPath: 0:不带路径，1：带路径
 * allfilename: 存储文件名的数组
 * return: 文件个数 
 * Copyright (c) 2023 by windsorH/hfy472309568@gmail.com, All Rights Reserved.
 */

#ifndef get_filenames_in_dir_h
#define get_filenames_in_dir_h
#define GET_FILENAME_MAX_LENGTH 256 // 文件名最大长度
#define GET_FILENAME_MAX_NUM 400    // 文件名最大个数
#ifdef __cplusplus
extern "C"
{
#endif
    int get_num_and_names_of_files_in_dir(char *path, char *type, int withPath, char (*allfilename)[GET_FILENAME_MAX_LENGTH]);
    int get_personal_info_from_files(char* filename, float* data);
#ifdef __cplusplus
}
#endif
#endif
