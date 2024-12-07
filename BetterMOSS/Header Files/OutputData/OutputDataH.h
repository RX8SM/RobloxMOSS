#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <algorithm> 
#include "..\miniz\miniz.h"

// Function responsible for outputting data to a zip file.
void _outPut2Zip(const std::wstring& newData, const char* zipFile, const char* fileName);