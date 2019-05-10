#include "head.h"

/*
	1.由于本人水平有限,逻辑较为混乱,如果看不懂,实属正常.
	2.增删改会修改成员数组,修改以后会重新写入文件中,再在下次加载初始化界面的时候重新读取数组.
	3.num是成员信息的个数,id是成员数组中最大的id加1. 
	4.该家谱管理系统会自动为添加的成员信息加上编号,编号可以通过[9  输出所有成员信息(测试用)]来查询.
	 
*/

int id = 0;//下一个成员的编号.
int num = 0;//当前成员数组的个数 
node people[50];//用于存储每一个成员的信息,最多可以有50个人.
int  now = 2019;//表示今年,用来计算成员的年龄 

//初始界面 
void display(){
	//初始化就读取文件,取得成员数组 
	ReadFile();
	
	system("cls");
	printf("\n");
	printf("                 **************************************\n");
	printf("                 *                                    *\n");
	printf("                 *         欢迎进入家谱管理系统       *\n");
	printf("                 *                                    *\n");
	printf("                 **************************************\n");
	printf("\n\n");
	printf("                         1  清空数据,初始化数组\n");
	printf("                         2  添加成员信息\n");
	printf("                         3  修改成员信息\n");
	printf("                         4  删除成员信息\n");
	printf("                         5  查询特定成员基本信息\n");
	printf("                         6  查询特定成员家庭信息\n");
	printf("                         7  显示家族树\n");
	printf("                         8  统计成员的基本信息\n");
	printf("                         9  输出所有成员信息(测试用)\n");
	printf("                        10  退出系统\n");

	printf("\n");
	
	int option;
	printf("您的选项是(输入数字):") ;
	scanf("%d",&option);
	while(option < 1 || option > 11){
		printf("输入错误,请重新输入!\n");
		printf("您的选项是(输入数字):");
		scanf("%d",&option);
		getchar();
	}
	switch(option){
		case 1:
			Initialize();
			break;
		case 2:
			AddMemberInfo();
			break;
		case 3:
			ModifierMemberInfo();
			break;
		case 4:
			DeleteMemberInfo();
			break;
		case 5:
			QueryMemberBasicInfo();
			break;
		case 6:
			QueryMemberFamilyInfo();
		case 7:
			if(num==0){
				system("cls"); 
				printf("家族树为空\n");
				printf("\n按任意键返回主菜单..\n");
				getch();
				system("cls");
				display();
			}else{
				DisplayFamilyTree(0);
			}
			break;
		case 8:
			countMemberInfo();
			break;
		case 9:
			DisplayMemberInfo();
			break; 
		case 10:
			exit(1);
			break;
	}
	
}

//清空数据,初始化成员数组
void Initialize(){
	system("cls"); 
	//以"w"打开文件,然后直接关闭,也就是清空文件内容 
	FILE *fp = fopen("information.txt","w");
	fclose(fp);
	//将id,num初始化 
	id = 0;
	num = 0;
	printf("数据清空完毕,成员数组已初始化!\n\n\n\n");
	
	printf("\n按任意键返回主菜单..\n");
	getch();
	system("cls");
	display();
} 

//读取文件内容
void ReadFile(){
	FILE *fp = fopen("information.txt","r");
	
	if(fp){
		for(num=0;!feof(fp);num++)
			fread(&people[num],sizeof(node),1,fp);
		//这里不知道为什么就是会多加一个,让它减掉一就可以了!!!!!!!!!!!!!!!!!!!!!!!(是个疑问) 
		num--;
		
		if(num==0){
			id = 0;
		} else{
			int i,max=0;
			for(i=0;i<num;i++){
				if(people[i].id>max)
					max = people[i].id;
			}
			id = max+1;
		}
//		printf("id:%d,num:%d",id,num);
//		getchar();
		fclose(fp);
		printf("读取文件成功!\n");
	}	
}

//查询id编号在成员数组中的下标,没有找到返回-1 
int QueryMemberIndex(int id){
	int i;
	for(i=0;i<num;i++){
		if(people[i].id == id)
			return i;
	}
	return -1;
} 

//查询id编号没有没有在孩子树head里面,1表示在,0表示不在 
int QueryChildrenId(int id,int head){
	if(head == id){
		return 1;
	}
	
	int i,index;
	index = QueryMemberIndex(head);
	for(i=0;i<people[index].aescendantnum;i++){
		if(QueryChildrenId(id,people[index].aescendant[i]) == 1)
			return 1;
	}
	return 0;
} 

//添加成员信息 
void AddMemberInfo(){
	int isLive; 
	
	people[num].id = id; 
	system("cls");
	getchar();
	printf("请输入名字:");
	gets(people[num].name);
	while(IsNameRepeate(people[num].name)==1){
		printf("输入名字重复,请检查重新输入!\n");
		printf("请输入名字:");
		gets(people[num].name);
	}
	printf("请输入出生地:");
	gets(people[num].birthplace);
	printf("请输入出生日期(格式如19990106):");
	gets(people[num].birthday);
	printf("是否健在?(0死亡,1健在)");
	scanf("%d",&isLive);
	getchar();
	if(!isLive){
		printf("这是一个不幸的消息.希望你不要太过悲伤.\n");
		printf("但我需要你提供死亡日期(格式如19990106):");
		gets(people[num].deathday); 
		getchar();
	}else{
		strcpy(people[num].deathday,"live");
	}
	printf("请输入性别(f是女,m是男):");
	scanf("%c",&people[num].sex);
	printf("请输入身高:");
	scanf("%f",&people[num].stature);
	getchar();
	printf("请输入学历:");
	gets(people[num].education);
	printf("请输入职业:");
	gets(people[num].profession);
	
	//初始化后代数为0
	people[num].aescendantnum = 0; 
	
	//计算年龄,死亡的用死亡年减出生年,健在的用now(今年)减出生年 
	char birth[4];
	strncpy(birth, people[num].birthday, 4);
	if(!isLive){
		char death[4];
		strncpy(death, people[num].deathday, 4);
		people[num].age = atoi(death) - atoi(birth); 
	}else{
		people[num].age = now - atoi(birth);
	}
	
	//若num为0,即为第一个结点,那么他就是始祖,他的parentid就是-1,表示空 
	if(num==0){
		people[num].parentid = -1;
	}else{
		printf("请输入父母的编号:");
		scanf("%d",&people[num].parentid);
		//如果父母编号没有在树中,就要重新输入. 
		while(QueryChildrenId(people[num].parentid,0)==0){
			printf("输入的父母编号错误,请检查后重新输入!\n");
			printf("请输入父母编号:");
			scanf("%d",&people[num].parentid);
		}
		
		//为他的父母的后代数组里添加元素
		int pid = people[num].parentid;
		int pindex = QueryMemberIndex(pid);
		people[pindex].aescendant[people[pindex].aescendantnum++] = id;
	}
	
	//计算代
	//如果是第一代就是1 
	if(num==0){
		people[num].generation = 1;
	}else{
		//如果不是第一代就是父母辈的代加一 
		int i = QueryMemberIndex(people[num].parentid);
		people[num].generation = people[i].generation + 1;
	}
	
	//用户添加完毕,num自加1 
	num++;
	//把成员数组信息写入文件中 
	WriteToFile(); 
	
	printf("\n按任意键返回主菜单..\n");
	getch();
	system("cls");
	display();
}

//修改后代的代的信息 
void ChangeGeneration(int id){
	int index = QueryMemberIndex(id);
	int pindex = QueryMemberIndex(people[index].parentid);
	people[index].generation = people[pindex].generation + 1;
	
	int i;
	for(i=0;i<people[index].aescendantnum;i++){
		ChangeGeneration(people[index].aescendant[i]);
	}
} 

//修改成员信息
void ModifierMemberInfo(){
	system("cls");
	
	int found=0;//用来表示有没有找到要修改的成员
	int i,j;//遍历用 
	int pid;//修改父母后代数组用 
	char name[20];//要修改成员的姓名 
	
	char birth[4];//出生年份
	char death[4];//死亡年份 
	
	printf("请输入要修改成员的姓名\n");
	getchar(); 
	gets(name);//取得要修改成员的名字 
	for(i=0;i<num;i++){
		if(strcmp(people[i].name,name)==0){
			found = 1;
			printf("你可以修改以下信息!\n");
			printf("1.父母编号   2.姓名   3.出生地   4.出生日期   5.死亡日期\n");
			printf("6.性    别   7.身高   8.学  历   9.职    业\n");
			int option;
			printf("请输入想要修改信息的编号:"); 
			scanf("%d",&option);
			while(option < 1 | option > 9){
				printf("输入错误,请重新输入!");
				printf("请输入想要修改信息的编号:"); 
				scanf("%d",&option);
			}
			getchar();
			switch(option){
				case 1:
					//为他原来的父母后代数组里面删除后代 
					pid = people[i].parentid;
					//父母在数组中的下标
					int m = QueryMemberIndex(pid); 
					
					for(j=0;j<people[m].aescendantnum;j++){
						if(people[m].aescendant[j] == i){
							break;
						}
					}
					j++;
					for(;j<=people[m].aescendantnum;j++){
						people[m].aescendant[j-1] = people[m].aescendant[j];
					}
					people[m].aescendantnum--;
					//重新输入父母编号 
					printf("请输入正确的父母编号:");
					scanf("%d",&people[i].parentid);
					//不允许把父母编号改为自己的子代,防止出现圆形家族谱 
					while(QueryChildrenId(people[i].parentid,people[i].id)==1){
						printf("父母编号错误,请重新输入!\n");
						printf("请输入孩子们的父母编号:");
						scanf("%d",&people[i].parentid);
					}
					
					//为他的父母的后代数组里添加后代 
					pid = people[i].parentid;
					m = QueryMemberIndex(pid); 
					people[m].aescendant[people[m].aescendantnum++] = i;
					
					//重新计算代 
					ChangeGeneration(people[i].id);
					
					printf("父母编号修改成功!\n");
					break;
				case 2:
					printf("请输入正确的姓名:"); 
					gets(people[i].name);
					while(IsNameRepeate(people[i].name)==1){
						printf("输入名字存在,请检查后重新输入!\n");
						printf("请输入正确的姓名:");
						gets(people[i].name);
					}
					printf("姓名修改成功!\n");
					break;
				case 3:
					printf("请输入正确的出生地:");
					gets(people[i].birthplace);
					printf("出生地修改成功!\n");
					break;
				case 4:
					printf("请输入正确的出生日期(格式如19990106):");
					gets(people[i].birthday);
					//除了修改出生日期还要修改年龄
					strncpy(birth, people[i].birthday, 4);
					if(strcmp(people[i].deathday,"live")==0){
						people[i].age = now - atoi(birth);
					}else{
						strncpy(death, people[i].deathday, 4);
						people[i].age = atoi(death) - atoi(birth); 
					}
					printf("出生日期修改成功!\n");
					break;
				case 5:
					printf("请输入正确的死亡日期(格式如19990106):");
					gets(people[i].deathday);
					//除了修改死亡日期还要修改年龄 
					strncpy(birth, people[i].birthday, 4);
					strncpy(death, people[i].deathday, 4);
					people[i].age = atoi(death) - atoi(birth); 
					printf("死亡日期修改成功!\n");
					break;
				case 6:
					printf("请输入正确的性别(f是女,m是男):");
					scanf("%c",&people[i].sex);
					while(people[i].sex != 'f' && people[i].sex != 'm'){
						printf("输入的性别错误,请检查后重新输入!\n");
						printf("请输入正确的性别(f是女,m是男):");
						scanf("%c",&people[i].sex);
					} 
					printf("性别修改成功!\n");
					break;
				case 7:
					printf("请输入正确的身高:");
					scanf("%f",&people[i].stature);
					printf("身高修改成功!\n");
					break;
				case 8:
					printf("请输入正确的学历:");
					gets(people[i].education); 
					printf("学历修改成功!\n");
					break;
				case 9:
					printf("请输入正确的职业:");
					gets(people[i].profession); 
					printf("职业修改成功!\n");
					break;
			}
		}
	}
	if(found == 0){
		printf("没有找到姓名为%s,请检查信息!\n\n",name); 
	}else{
		printf("是否继续修改?(1继续0不继续):");
		int goon;
		scanf("%d",&goon);
		if(goon == 1){
			ModifierMemberInfo();
		} 
	}

	//修改了信息以后,需要重新写入文件中.
	WriteToFile();
	
	printf("\n按任意键返回主菜单..\n");
	getch();
	system("cls");
	display();
} 


//删除成员信息(不允许删除根结点) 
void DeleteMemberInfo(){
	system("cls");
	getchar();
	char deletename[20];
	printf("请输入需要删除的成员姓名：");
	gets(deletename);
	int i,j,m;//i是需要删除成员在数组的下标,j是父母的孩子数组中删除成员的下标,m是父代在数组的下标 
	for(i=0;i<num;i++){
		if(strcmp(deletename,people[i].name)==0)
			break;
	}
	if(i == id){
		printf("未查询到该姓名的成员，删除失败！\n");
		printf("\n按任意键返回主菜单..\n");
		getch();
		system("cls");
		display();
	}else{
		//为他的父母删除他这个成员
		int pid = people[i].parentid;
		//父母的下标 
		m = QueryMemberIndex(pid); 
		//父母孩子数组中的要删除的成员 
		for(j=0;j<people[pid].aescendantnum;j++){
			if(people[m].aescendant[j] == i){
				break;
			}
		}
		//删除父母孩子数组中的成员 
		j++;
		for(;j<people[m].aescendantnum;j++){
			people[pid].aescendant[j-1] = people[pid].aescendant[j];
		}
		people[pid].aescendantnum--;
		
		//如果他有孩子
		if(people[i].aescendantnum!=0){
			printf("请输入孩子们的父母编号:");
			scanf("%d",&pid);
			//不允许把父母编号改为比原来要大的,防止出现圆形家族谱 
			while(QueryChildrenId(pid,people[i].id)==1){
				printf("父母编号错误,请重新输入!\n");
				printf("请输入孩子们的父母编号:");
				scanf("%d",&pid);
			}
			
			//新的父母的下标 
			m = QueryMemberIndex(pid);
			
			for(j=0;j<people[i].aescendantnum;j++){
				//改变孩子们的父母 
				int cid = people[i].aescendant[j];//孩子id
				int n = QueryMemberIndex(cid);//n是孩子的数组下标
				people[n].parentid = pid;
				
				//更新孩子们的代
				ChangeGeneration(cid);
				
				// 更新父母的孩子
				people[m].aescendant[people[m].aescendantnum++] = cid;	
			} 
		}
		
		//在成员数组中删除这个成员
		i++;
		for(;i<num;i++){
			people[i-1] = people[i];
		}
		num--;
		
		//把成员数组的信息写回文件中 
		WriteToFile(); 
	}
	
	printf("删除成功!"); 
	printf("\n按任意键返回主菜单..\n");
	getch();
	system("cls");
	display();
} 

//查询成员基本信息
void QueryMemberBasicInfo(){
	system("cls");
	int found; 
	char name[20];
	printf("请输入需要查询成员的姓名:");
	getchar();
	gets(name); 
	int i; 
	for(i=0;i<num;i++){
		if(strcmp(name,people[i].name)==0){
			found = 1;
			printf("成员查询成功,基本信息如下:\n"); 
			printf("***************************\n");
			printf("编    号 : %d\n",people[i].id);
			printf("父母编号 : %d\n",people[i].parentid);
			printf("姓    名 : %s\n",people[i].name);
			printf("出 生 地 : %s\n",people[i].birthplace);
			printf("出生日期 : %s\n",people[i].birthday);
			if(strcmp(people[id].deathday,"live")!=0){
				printf("死亡日期 : %s\n",people[i].deathday);
			}
			if(people[i].sex=='m'){
				printf("性    别 : 男\n");
			}else if(people[i].sex=='f'){
				printf("性    别 : 女\n");
			}
			
			printf("身    高 : %.2f\n",people[i].stature);
			printf("学    历 : %s\n",people[i].education);
			printf("职    业 : %s\n",people[i].profession);
			printf("年    龄 : %d\n",people[i].age);
			printf("   代    : %d\n",people[i].generation);
			printf("***************************\n");
			printf("\n\n");
			break;
		} 
	}
	
	if(found == 0){
		printf("对不起,未找到该成员,请检查信息!\n");
	}
	
	printf("\n按任意键返回主菜单..\n");
	getch();
	system("cls");
	display();
} 

//查询成员家庭信息
void QueryMemberFamilyInfo(){
	system("cls");
	getchar(); 
	printf("请输入要查询成员的姓名:") ;
	char name[20];
	gets(name);
	int i;
	for(i=0;i<num;i++){
		if(strcmp(name,people[i].name)==0)
			break;
	}
	if(i==id){
		printf("未找到该成员,查询失败!\n");
		printf("\n按任意键返回主菜单..\n");
		getch();
		system("cls");
		display();
	}
	
	printf("该成员的父母编号:%d\n",people[i].parentid);
	int index = QueryMemberIndex(people[i].parentid);
	printf("该成员的父母姓名为:%s\n",people[index].name); 
	printf("该成员在家族中属于第%d代\n",people[i].generation);
	if(people[i].aescendantnum==0){
		printf("该成员没有子女\n"); 
	} else{
		printf("该成员的子女编号及姓名为:");
		int j;
		for(j=0;j<people[i].aescendantnum;j++){
			printf("\n"); 
			printf("编号:%d  ",people[i].aescendant[j]);
			index = QueryMemberIndex(people[i].aescendant[j]);
			printf("姓名:%s",people[index].name);
		} 
		printf("\n");
	}
	
	printf("查询成功!\n");
	printf("\n按任意键返回主菜单..\n");
	getch();
	system("cls");
	display();
} 

//显示家族树递归 
void DisplayTree(int id){
	
	int i,j=0;
	int index;
	//找到id在数组中的下标 
	index = QueryMemberIndex(id);
	
	for(i = 0;i<people[index].generation;i++){
		printf("|—");
	}
	printf("%d",id);
	
	printf("\n");
	while(j!=people[index].aescendantnum){
		DisplayTree(people[index].aescendant[j]);
		j++;
	}
		
} 

//显示家族树main
void DisplayFamilyTree(int id){
	system("cls");
	
	DisplayTree(id); 
	
	printf("\n按任意键返回主菜单..\n");
	getch();
	system("cls");
	display();
} 


//统计成员的基本信息
void countMemberInfo(){
	system("cls");
	int i;
	
	float avgAge=0;
	for(i=0;i<num;i++){
		avgAge += people[i].age;
	} 
	avgAge/=num;
	printf("平均寿命:%.2f\n",avgAge);
	
	
	float avgStature;
	for(i=0;i<num;i++){
		avgStature += people[i].stature;
	}
	avgStature/=num;
	printf("平均身高:%.2f\n",avgStature);
	
	int mNum=0,fNum=0;
	for(i=0;i<num;i++){
		if(people[i].sex == 'f')
			fNum++;
		else if(people[i].sex == 'm')
			mNum++;
	}
	float rate = (float)mNum / (fNum + mNum); 
	printf("男有%d人,女有%d人,男女比例%.2f\n",mNum,fNum,rate);
	
	float avgDescendant=0;
	for(i=0;i<num;i++){
		avgDescendant += people[i].aescendantnum;
	}
	avgDescendant/=num;
	printf("平均人后:%.2f\n",avgDescendant);
	
	printf("\n按任意键返回主菜单..\n");
	getch();
	system("cls");
	display();
} 


//输出所有成员的信息 
void DisplayMemberInfo(){
	system("cls");

	int i; 
	for(i=0;i<num;i++){
		printf("***************************\n");
		printf("编    号 : %d\n",people[i].id);
		printf("父母编号 : %d\n",people[i].parentid);
		printf("姓    名 : %s\n",people[i].name);
		printf("出 生 地 : %s\n",people[i].birthplace);
		printf("出生日期 : %s\n",people[i].birthday);
		if(strcmp(people[id].deathday,"live")!=0){
			printf("死亡日期 : %s\n",people[i].deathday);
		}
		if(people[i].sex=='m'){
			printf("性    别 : 男\n");
		}else if(people[i].sex=='f'){
			printf("性    别 : 女\n");
		}
		
		printf("身    高 : %.2f\n",people[i].stature);
		printf("学    历 : %s\n",people[i].education);
		printf("职    业 : %s\n",people[i].profession);
		printf("年    龄 : %d\n",people[i].age);
		printf("   代    : %d\n",people[i].generation);
		printf("后 代 数 : %d\n",people[i].aescendantnum);
		printf("后代编号 :");
		int j;
		for(j=0;j<people[i].aescendantnum;j++){
			printf(" %d",people[i].aescendant[j]);
		} 
		printf("\n");
		printf("***************************\n");
		printf("\n\n"); 
	} 
	
	printf("\n按任意键返回主菜单..\n");
	getch();
	system("cls");
	display();
} 

//判断成员姓名是否重复 
int IsNameRepeate(char name[]){
	int i;
	for(i=0;i<num;i++){
		if(strcmp(people[i].name,name)==0)
			return 1;
	}
	return 0;
}

//把成员数组的数据写入文件中
void WriteToFile(){
	FILE *fp = fopen("information.txt","w");
	int i;
	for(i=0;i<num;i++){
		fwrite(&people[i],sizeof(node),1,fp);
	}
	fclose(fp);
} 