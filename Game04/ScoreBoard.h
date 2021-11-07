#pragma once

#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <thread>
#include <mutex>

struct ScoreData
{
	char name[20];
	int score;
}data[6];

void scoreRead(const char* filename) 
{
	FILE* fp = NULL;
	int err = fopen_s(&fp,filename, "r");
	if (err)
	{
		fopen_s(&fp, filename, "w");
		return;
	}
	for (int i = 0; i < 5 && !feof(fp) && fp != NULL; i++)
	{
		fscanf_s(fp,"%[^,],",data[i].name,_countof(data[i].name));
		fscanf_s(fp,"%d\n",&data[i].score);
	}
	fclose(fp);
}

void scoreSort()
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = i + 1; j < 6; j++)
		{
			if (data[j].score > data[i].score)
			{
				ScoreData temp = data[i];
				data[i] = data[j];
				data[j] = temp;
			}
		}
	}
}

void scoreAdd(const char* filename, const char* playername, int score)
{
	FILE* fp = NULL;
	fopen_s(&fp, filename, "w");
	strcpy_s(data[5].name,19,playername);
	data[5].score = score;
	scoreSort();
	for (int i = 0; i < 5 && fp!= NULL ; i++)
	{
		fprintf_s(fp, "%s,%d\n", data[i].name, data[i].score);
	}
	fclose(fp);
}


