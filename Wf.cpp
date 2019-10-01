#include<stdio.h>
#include<iostream>
#include<vector>
#include<map> 
#include<algorithm>
#include<cstring>
#include<iomanip>  
#include<ctype.h>
#include<fstream>
#include<io.h>
using namespace std;

/*��ĸ����Ƶ������sort( , , cmp)*/
bool cmp(pair<char,float> a, pair<char,float> b) {
	return a.second > b.second;
}
/*���ʳ��ִ�������sort( , ,cmp1)*/ 
bool cmp1(pair<string,int> a, pair<string,int> b) {
	return a.second > b.second;
}
/*����Ŀ¼��ȡ�ļ���*/
void GetFilesName(string path,vector<string>& files,char flag)
{
    intptr_t  hFile = 0;
    struct _finddata_t fileinfo;
    string p;
    hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo);
    if (hFile != -1){
    	if(flag == 'd'){
    		do{
    			if((!strcmp(fileinfo.name,"."))||(!strcmp(fileinfo.name,"..")))
    				continue;
    			files.push_back(p.assign(path).append("/").append(fileinfo.name));
			}while (_findnext(hFile, &fileinfo) == 0);
		}
		else{
			do{
	            if ((fileinfo.attrib &  _A_SUBDIR)){
	                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
	                    GetFilesName(p.assign(path).append("/").append(fileinfo.name),files,'s');
	            }
	            else
	            {
	            	if((!strcmp(fileinfo.name,"."))||(!strcmp(fileinfo.name,"..")))
    					continue;
	                files.push_back(p.assign(path).append("/").append(fileinfo.name));
	            }
        	} while (_findnext(hFile, &fileinfo) == 0);
		}
        _findclose(hFile);
    }
	else{
		cout<<"Ŀ¼������"<<endl; 
	}
}

/*���������в�����ȡ�ļ���*/ 
map<string,string> findfile(int argc,char *argv[]){
	map<string,string> arg;
	string temp;
	for(int i=1;i<argc;){
		int j = i+1;
		temp = "";
		if(argv[i][0] == '-'){
			for( ;(argv[j][0]!='-')&&(j<argc-1);j++){
				temp = temp + argv[j] + " ";
			}
			if(j==(argc-1))
				temp = temp + argv[j];
			temp.erase(temp.find_last_not_of(" ") + 1);;
			arg.insert(pair<string,string>(argv[i],temp));
		}
		i = j;
	}
	return arg;
}

/*����file�ļ�����ĸ���ֵ�Ƶ��*/
void alpFrequent(map<string,string> arg){
	string file;
	FILE *fp;
	int results[26];
	vector<pair<char,float> > vec;
	int sum=0;
	float percent;
	map<string,string>::iterator it;
	it = arg.find("-c");
	file = it->second;
	try{
			fp = fopen(file.c_str(),"r") ;
			if(fp == NULL)
				cout<<"�ļ�������"<<endl;
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
				cout<<"��ĸʹ��Ƶ������Ϊ��"<<endl; 
				for(int i=0;i<26;i++){
					cout<<vec[i].first<<":"<<setiosflags(ios::fixed) << setprecision(2) <<vec[i].second<<"%"<<endl;
				}
			}
	}
	catch(char *e){
		cout<<e<<endl;
	}
}

/*���ַ�����д��ĸ��ΪСд��ĸ*/ 
string toLower(string word){
	int i=0;
	while(word[i]){
		if(word[i]>='A'&&word[i]<='Z')
			word[i] = tolower(word[i]);
		i++;
	}
	return word;
}

/*�����°��зָ�ɵ���*/
vector<string> split(string line,int number){
	vector<string> words;
	string word;
	int i = 0;
	int space;
	while(i<line.size()){
		space = 0;
		while(i<line.size()&&(!isalpha(line[i]))){
			i++;
		}
		int j=i;
		while(j<line.size()&& (isalpha(line[j])||isdigit(line[j])||isspace(line[j]))&&(space<number)){
			if(isspace(line[j]))
				space++;
			j++;
		}
		if(i!=j){
			word = line.substr(i, j-i);
			words.push_back(toLower(word));
			i=j;
		}
	}
	return words;
}

/*���㲻ͬ���ʵĳ���Ƶ��*/ 
void wordsFrequent(map<string,string> arg){
	map<string,int> counts;
	string line;
	vector<string> stopwords;
	vector<string> words;
	vector<string> verbs; 
	ifstream in;
	ifstream stop;
	ifstream verb;
	
	/*����map�еĲ���*/
	map<string,string>::iterator file;
	file = arg.find("-f");
	
	map<string,string>::iterator stopfile;
	stopfile = arg.find("-x");
	
	map<string,string>::iterator verbfile;
	verbfile = arg.find("-v");
	
	map<string,string>::iterator length;
	int leng=1;
	length = arg.find("-p");
	if(length!=arg.end())
		leng = abs(atoi((length->second).c_str()));
		
	map<string,string>::iterator number;
	int n=0;
	number = arg.find("-n");
	if(number!=arg.end())
		n = abs(atoi((number->second).c_str()));
			
	try{
			if(stopfile!=arg.end()){
				stop.open((stopfile->second).c_str());
				if(!stop.is_open()){
					cout<<"ͣ���ļ�������"<<endl;
					return; 
				}
				while(getline(stop,line)){
					vector<string> tempstop = split(line,1);
					stopwords.insert(stopwords.end(),tempstop.begin(),tempstop.end());
				}
			}
			in.open((file->second).c_str());
			if(!in.is_open())
				cout<<"�ļ�������"<<endl;
			else{
					while(getline(in,line)){
						words=split(line,leng);
						for(vector<string>::iterator it=words.begin();it!=words.end();++it ){
							if(stopfile!=arg.end()){
								for(vector<string>::iterator stopIt=stopwords.begin();stopIt!=stopwords.end();stopIt++){
									if (*it==*stopIt){
										break;
									}
									else{
										if(stopIt==(stopwords.end()-1)){
											counts[*it]++;
										}
									}
								}
							}
							else
								counts[*it]++;
						}
					}
			}
			if(verbfile != arg.end()){
				verb.open((verbfile->second).c_str());
				if(!verb.is_open()){
					cout<<"���ʱ����ļ�������"<<endl;
					return; 
				}
				while(getline(verb,line)){
					vector<string> tempverb = split(line,1);
					verbs.insert(verbs.end(),tempverb.begin(),tempverb.end());
				}
				int sum = verbs.size()/4;
				map<string,int>::iterator findverb;
				for(int i=1;i<4;i++){
					for(int j=0;j<sum;j++){
						findverb = counts.find(verbs[4*j+1]);
						if(findverb!= counts.end()){
							if(verbs[4*j]!=verbs[4*j+1]){
								counts[verbs[4*j]] += findverb->second;
								counts.erase(findverb);
							}
						}
					}
				}
			}
			vector<pair<string,int> > results;
			copy(counts.begin(), counts.end(), back_inserter(results));
			sort(results.begin(),results.end(),cmp1);
			cout<<file->second<<"�е��ʵ�ʹ�ô�����"<<endl;
			if(!n){
				for(int i=0;i<results.size();i++){
				cout<<results[i].first<<":"<<results[i].second<<"��"<<endl;
			}
			}
			else{
				for(int i=0;i<n;i++){
				cout<<results[i].first<<":"<<results[i].second<<"��"<<endl;
				}
			}
		}
	catch(char *e){
		cout<<e<<endl;
	}
	return;
}

int main(int argc,char *argv[]) {
	map<string,string> arg;
	string filename;
	string stop;
	int n=0;
	int number=1;
	vector<string> files;
	map<string,string>::iterator it;	
	if(argc >= 3){
		arg = findfile(argc,argv);	
		if(arg.find("-c")!=arg.end()){
				
				alpFrequent(arg);	
		}
		else if(arg.find("-f")!=arg.end()){
			wordsFrequent(arg);
		}
		else if(arg.find("-d")!=arg.end()){
			it = arg.find("-d");
			string root=it->second;
			GetFilesName(root,files,'d');
			for(vector<string>::iterator it=files.begin();it!=files.end();it++){
				arg["-f"] = (*it);
				wordsFrequent(arg);
			}
		}
		else if(arg.find("-s")!=arg.end()){
			it = arg.find("-s");
			string root=it->second;
			GetFilesName(root,files,'s');
			for(vector<string>::iterator it=files.begin();it!=files.end();it++){
				arg["-f"] = (*it);
				wordsFrequent(arg);
			}
		}
		else{
		cout<<"��ѡ�������ʽΪ�� "<<endl; 
		cout<<"-c:�����ļ���ĸʹ��Ƶ�� "<<endl;
		cout<<"-f:�����ļ�����ʹ�ô��� "<<endl;
		cout<<"-d(-s):����Ŀ¼(��Ŀ¼)�ļ��µ���ʹ�ô��� "<<endl;
		cout<<"-n:ʹ�ô�������n������ "<<endl;
		cout<<"-p:p��������ɵĶ����ʹ�ô��� "<<endl;
		cout<<"-s:��ȥͣ���ļ��еĵ��ʣ��������ʵ�ʹ�ô��� "<<endl;
		cout<<"-v:�����ʶ��ϲ�Ϊԭ�ͼ������ "<<endl;
	}
	}
	else{
		cout<<"�����ʽΪ��wf.exe -x xxxxxxx "<<endl; 
	}
	return 0;
}
