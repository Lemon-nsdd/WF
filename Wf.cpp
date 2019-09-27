#include<stdio.h>
#include<iostream>
#include<vector>
#include<map> 
#include<algorithm>
#include<cstring>
#include<iomanip>  
using namespace std;

bool cmp(pair<char,float> a, pair<char,float> b) {
	return a.second > b.second;
}
 
void alpFrequent(int argc,char *argv[]){
	FILE *fp;
	char location[50];
	string rel="data/"; 
	int results[26];
	vector<pair<char,float> > vec;
	int sum=0;
	float percent;
	for(int i=2;i<argc;i++){
		rel = rel + argv[i] + " ";
	}
	strcpy(location,rel.c_str());
	try{
			fp = fopen(location,"r") ;
			if(fp == NULL)
				cout<<"文件不存在"<<endl;
			else{	
				char ch;
				for(int i=0;i<26;i++){
					results[i] = 0;
				}
				while(fscanf(fp,"%c",&ch)!=EOF){
					if(ch>='a'&&ch<='z')
					{
						results[ch-'a']++;
						sum++;
					}
					else if(ch>='A'&&ch<='Z')
					{
						results[ch-'A']++;
						sum++;
					}
					else
						continue;
				}
				
				for(int i=0;i<26;i++){
					percent = (float)results[i]*100/sum;
					vec.push_back(pair<char,float>(i+97,percent));
				}
				sort(vec.begin(),vec.end(),cmp);
				cout<<"字母使用频率排序为："<<endl; 
				for(int i=0;i<26;i++){
					cout<<vec[i].first<<":"<<setiosflags(ios::fixed) << setprecision(2) <<vec[i].second<<"%"<<endl;
				}
			}
	}
	catch(char *e){
		cout<<e<<endl;

	}

	

}
int main(int argc,char *argv[]) {	
	if(argc >= 3){	
		if(!strcmp(argv[1],"-c")){
			alpFrequent(argc,argv);	
		}
		else{
		cout<<"命令格式为：wf.exe -c <file name> "<<endl; 
	}
	}
	else{
		cout<<"命令格式为：wf.exe -x xxxxxxx "<<endl; 
	}
	return 0;
}
