#include<stdio.h>
#include<ctime>
#include<cstdlib>
#include"Sudoku.h"

void Sudoku::giveQuestion(){
	printf("0 0 0 0 0 0 0 2 0\n");
	printf("1 0 0 9 0 0 0 0 0\n");
	printf("4 0 0 0 0 0 0 0 0\n");
	printf("0 0 8 0 0 2 0 0 0\n");
	printf("0 0 0 0 0 0 0 0 1\n");
	printf("0 0 0 0 0 0 9 0 4\n");
	printf("0 0 5 0 0 0 3 0 0\n");
	printf("0 0 0 1 0 0 0 0 7\n");
	printf("0 0 2 4 0 8 0 0 0\n");
	return;
}

void Sudoku::readIn(){
	int i;
	for(i=0;i<81;++i)
		scanf("%d",&map[i]);
	return;
}

void Sudoku::solve(){
	int row,col1,col2,col3,col4;
	int correct[81];
	int countcor;
	countcor = 0;
	
	head = 0;
	flag = 0; //初始化為無解
	for(int i=0; i<=9*9*4; ++i) //初始化column object的dlx node
	{
		dlx[i].num = 0; //1的個數初始化為0
	    dlx[i].U = i; //column object的上下指標初始化指回自己
		dlx[i].D = i;
		dlx[i].R = i+1;//column object的左右指標串起來
		dlx[i].L = i-1;
	}
	size = 9*9*4; //記錄前9*9*4的interger為column object使用
	dlx[9*9*4].R = 0; //最後面的指標拉回指向head

	for(int i=0,k=0;i<9;++i) //k為0~81之位置，i,j為k的對應列行值
	{	
		for(int j=0;j<9;++j,++k)
		{
			if(map[k]>0) //題目給的確定數字
			{
				exist[i][j] = 1;
				//將其轉換成0-1矩陣的'1'節點
				row = (i*9+j)*9 + map[k]; //其對應之列值
				col1 = i*9 + j + 1; //一格只能有一個數字
				col2 = 9*9 + i*9 + map[k]; //一列只能出現一次
				col3 = 9*9*2 + j*9 + map[k]; //一行只能出現一次
				col4 = 9*9*3 + ((i/3)*3+(j/3))*9 + map[k]; //一個block只能出現一次
				//再將其插入0-1矩陣中
				insert(row, col1, col2, col3, col4);
				correct[countcor++] = i*9+j+1;
			}
		}
	}
	//插入一般待解的'1'進入0-1矩陣
	for(int i=0;i<9;++i)
	{
		for(int j=0;j<9;++j)
		{
			if(exist[i][j] == 1) continue;
			for(int k=1;k<=9;++k)
			{
			   row = (i*9+j)*9 + k;
               col1 = i*9 + j + 1; 
               col2 = 9*9 + i*9 + k; 
               col3 = 9*9*2 + j*9 + k;
               col4 = 9*9*3 + ((i/3)*3+(j/3))*9 + k;
			   insert(row, col1, col2, col3, col4);
			}
		}
	}
	if(countcor<30&&countcor>15)
	{
	for(int k=0;k<countcor;++k) 
	{
	    remove(correct[k]);
	    int i = dlx[correct[k]].D; //i是移除後下方的'1'節點，去往左右找有'1'之節點做移除
	    while(i != correct[k])
	    {
		    int j = dlx[i].R;
		    while(j != i)
	        {
			    remove(dlx[j].colobj); //同一列有'1'之column object就不能選了
			    j = dlx[j].R;
		    }
		    i = dlx[i].D;
       	}
	}
	}
	dfs(0);
	if(flag==0) printf("0\n"); //無解
	else if(flag>1) printf("2\n"); //多組解
	else //有解
	{
		printf("1\n");
		for(int i=0; i<81; ++i)
		{
			printf("%d%c",map[i],(i+1)%9==0?'\n':' ');
		}
	}

}

void Sudoku::changeNum(int a, int b){ 
	int temp[81];
	if(a==b) return;
	for(int i=0;i<81;++i)
	{
		if(map[i]==a) temp[i]=b;
		else if(map[i]==b) temp[i]=a;
		else temp[i]=map[i];
	}
	for(int i=0;i<81;++i) map[i]=temp[i];
	return;
}

void Sudoku::changeRow(int a, int b){ 
	int temp[81];
	if(a==b) return;
	if((a==0&&b==1)||(a==1&&b==0))
	{
		for(int i=0;i<81;++i)
		{
			if(i<27) temp[i+27]=map[i];
			else if(i<54) temp[i-27]=map[i];
			else temp[i]=map[i];
		}
		for(int i=0;i<81;++i) map[i]=temp[i];
		return;

	}
	if((a==0&&b==2)||(a==2&&b==0))
	{
	    for(int i=0;i<81;++i)
	    {
	        if(i<27) temp[i+54]=map[i];
	        else if(i<54) temp[i]=map[i];
	        else temp[i-54]=map[i];
	    }
		for(int i=0;i<81;++i) map[i]=temp[i];
		return;
	}
	if((a==2&&b==1)||(a==1&&b==2))
	{
	    for(int i=0;i<81;++i)
	    {
	        if(i<27) temp[i]=map[i];
	        else if(i<54) temp[i+27]=map[i];
	        else temp[i-27]=map[i];
	    }
		for(int i=0;i<81;++i) map[i]=temp[i];
		return;
	}
}

void Sudoku::changeCol(int a, int b){
	int temp[81];
	if(a==b) return;
	if((a==0&&b==1)||(a==1&&b==0))
	{
		for(int i=0;i<81;++i)
		{
			if((i%9)<3) temp[i]=map[i+3];
			else if((i%9)<6) temp[i]=map[i-3];
			else temp[i]=map[i];
		}
		for(int i=0;i<81;++i) map[i]=temp[i];
		return;
	}
	if((a==0&&b==2)||(a==2&&b==0))
	{
		for(int i=0;i<81;++i)
	    {
	        if((i%9)<3) temp[i]=map[i+6];
	        else if((i%9)<6) temp[i]=map[i];
	        else temp[i]=map[i-6];
	    }
	    for(int i=0;i<81;++i) map[i]=temp[i];
		return;
	}
	if((a==2&&b==1)||(a==1&&b==2))
	{
		for(int i=0;i<81;++i)
	    {
	        if((i%9)<3) temp[i]=map[i];
	        else if((i%9)<6) temp[i]=map[i+3];
	        else temp[i]=map[i-3];
	    }
	    for(int i=0;i<81;++i) map[i]=temp[i];
		return;
	}
}

void Sudoku::rotate(int n){
	int temp[81],save[81];
	if(n%4==0) return;
	for(int i=0;i<81;++i) save[i]=map[i];
	for(int k=0;k<(n%4);++k) //轉k次90度
	{
		for(int i=0;i<81;++i)
		{
			temp[i]=save[(i/9)+9*(8-(i%9))];
			/*
			 *if((i%9)==0) temp[i]=save[(i/9)+72];
			 *if((i%9)==1) temp[i]=save[(i/9)+63];
			 *...
			 *if((i%9)==8) temp[i]=save[(i/9)+0];
			 */
        }
		for(int i=0;i<81;++i) save[i]=temp[i];
	}
	for(int i=0;i<81;i++) map[i]=temp[i];
	return;
}

void Sudoku::flip(int n){
	int temp[81];
	if(n==0)
	{
		for(int i=0;i<81;++i)
		{
			if(i<9) temp[i]=map[i+72];
			else if(i<18) temp[i]=map[i+54];
			else if(i<27) temp[i]=map[i+36];
			else if(i<36) temp[i]=map[i+18];
			else if(i<45) temp[i]=map[i];
			else if(i<54) temp[i]=map[i-18];
			else if(i<63) temp[i]=map[i-36];
			else if(i<72) temp[i]=map[i-54];
            else temp[i]=map[i-72];
		}
		for(int i=0;i<81;++i) map[i]=temp[i];
		return;
	}
	else
	{
		for(int i=0;i<81;++i)
		{
			if(i%9==0) temp[i]=map[i+8];
			else if(i%9==1) temp[i]=map[i+6];
			else if(i%9==2) temp[i]=map[i+4];
			else if(i%9==3) temp[i]=map[i+2];
			else if(i%9==4) temp[i]=map[i];
			else if(i%9==5) temp[i]=map[i-2];
			else if(i%9==6) temp[i]=map[i-4];
			else if(i%9==7) temp[i]=map[i-6];
			else temp[i]=map[i-8];
		}
		for(int i=0;i<81;++i) map[i]=temp[i];
		return;
	}
}

void Sudoku::transform(){
	srand(time(NULL));
	changeNum(rand()%9+1, rand()%9+1);
	changeRow(rand()%3, rand()%3);
	changeCol(rand()%3, rand()%3);
	rotate(rand()%101);
	flip(rand()%2);
    for(int i=0;i<81;++i)  printf("%d%c",map[i],(i+1)%9==0?'\n':' ');
    return;
}

/*
 * private member function
 */

inline void Sudoku::remove(int col){
	/*
	 * 把選定的column object(編號col)移除
	 * 結構是以雙向鏈結儲存，將右邊的指標指向左邊
	 * 再將左邊的指標指向右邊
	 * 同時不真正釋放此節點，此節點之左右指標仍指向原先左右節點
	 * 故可在之後發現錯誤重新連回去
	 */
	dlx[dlx[col].R].L = dlx[col].L;
	dlx[dlx[col].L].R = dlx[col].R;
	int i = dlx[col].D; //i是被選中之column底下的第一個節點
	while(i != col) //如果往下指回自己，表示沒有'1'
	{
		//i現在指向選中之column底下的'1'節點
		int j = dlx[i].R; //j是i所指'1'節點右手邊的'1'節點
		while(j != i)
		{
			/*
			 * 選擇某一個'1'之後，右手邊有'1'皆不可被選
			 * 所以將同一列之所有'1'節點透過相同手法暫時移除
			 */
			dlx[dlx[j].D].U = dlx[j].U;
			dlx[dlx[j].U].D = dlx[j].D;
			dlx[dlx[j].colobj].num --; //該column object少了一個'1'
			j = dlx[j].R; //繼續往右邊找
		}
		i = dlx[i].D; //往下一個'1'節點做一樣的同一列'1'移除
	}
	return;
}


inline void Sudoku::resume(int col){
	/*
	 * 之前雖然被暫時移除了，但其上下左右指標仍存在
	 * 先指向其上方的'1'節點
	 * 因為先前將同列上的'1'節點都暫時移除了
	 * 所以現在由下而上的一一跑過這一column的'1'節點
	 * 去回復同一列的'1'節點之有效性
	 * 回復完之後指回自己的coiumn object再將左右的指標回復
	 */
	int i = dlx[col].U; 
	while(i != col)
	{
		//i指著由下而上的第一個節點
		int j = dlx[i].L;
		while(j != i)
		{
			//j指著欲回復的'1'節點
			dlx[dlx[j].U].D = j;
			dlx[dlx[j].D].U = j;
			dlx[dlx[j].colobj].num ++; //回復之後'1'加回來
			j = dlx[j].L; //繼續往左邊找'1'回復
		}
		i = dlx[i].U; //繼續往上找回復
	}
	//指回自己column object了，將左右指標回復
	dlx[dlx[col].R].L = col;
	dlx[dlx[col].L].R = col;
	return;
}

void Sudoku::dfs(int time){ 
	if(dlx[head].R == head)   	
	{
	//開頭向右指標指回開頭，表示已經沒有column object可以選擇
	//也就是已經都成功的選擇，即已有解
		if(flag>1) return;
		flag++; //有解將flag+1
		for(int i=0; i<time; ++i)
	    {
		    //將所紀錄之ans[]位置還原
		    map[(rowof[ans[i]]-1)/9] = (rowof[ans[i]])%9==0?9:(rowof[ans[i]])%9;
		}
		return;
	}
	//以下便為尚有column object可以選擇的情況
	int col;
	int temp = 9999999;
	int i = dlx[head].R; //從第一個開始
	while(i != head)
	{
		if(dlx[i].num < temp)
		{
			temp = dlx[i].num; //即找出擁有'1'最少節點的column object
			col = i; //並記錄下來優先移除，以有效減少branch個數
		}
		i = dlx[i].R; //每一個掃過找最小，直到指回head
	}
	//現在擁有最少'1'個數之column object編號:col
	//printf("the %d time choose %d col remove\n",time,col);
	remove(col);  //暫時移除
	i = dlx[col].D; //i是移除後下方的'1'節點，去往左右找有'1'之節點做移除
	while(i != col)
	{
		ans[time] = i; //記錄這一次所選之填入'1'之節點，即是答案
		int j = dlx[i].R;
		while(j != i)
		{
			remove(dlx[j].colobj); //同一列有'1'之column object就不能選了
			j = dlx[j].R;
		}
		//移除完不能被選擇的選項了
		dfs(time+1); //繼續往下做下一次的dfs
		if(flag>1) return;
		//若其中一層false，則為選錯column object
		j = dlx[i].L;
		while(j != i)
		{
			resume(dlx[j].colobj); //剛剛移除的回復
			j = dlx[j].L; //直到都回復
		}
		i = dlx[i].D;
	}
	resume(col); //把這個選錯的column object回復回來
	return; //傳回上層失敗，跳回上層繼續往下一個試
    //直到最外層也傳回失敗表示無解
}

inline void Sudoku::insert(int row, int col1,int col2,int col3,int col4){
	int temp1 = ++size; //指向已被使用的size範圍後面下一個
	int temp2 = ++size;
	int temp3 = ++size;
	int temp4 = ++size;
	dlx[temp1].colobj = col1; //該點隸屬於編號col 的 column object
	dlx[temp2].colobj = col2;
	dlx[temp3].colobj = col3;
	dlx[temp4].colobj = col4;
	dlx[col1].num++; //'1'的節點插入多一個
	dlx[col2].num++;
	dlx[col3].num++;
	dlx[col4].num++;
	rowof[temp1] = row;
	rowof[temp2] = row;
	rowof[temp3] = row;
	rowof[temp4] = row;
	dlx[temp1].D = dlx[col1].D; //將其向下指標指向原本指向的節點
	dlx[temp2].D = dlx[col2].D;
	dlx[temp3].D = dlx[col3].D;
	dlx[temp4].D = dlx[col4].D;
    dlx[dlx[col1].D].U = temp1; //再把指向的那個節點往回指
	dlx[dlx[col2].D].U = temp2; 
	dlx[dlx[col3].D].U = temp3; 
	dlx[dlx[col4].D].U = temp4; 
	dlx[temp1].U = col1; //將其向上的指標指向column object
	dlx[temp2].U = col2;
	dlx[temp3].U = col3;
	dlx[temp4].U = col4;
	dlx[col1].D = temp1; //再把column object也往下回指
	dlx[col2].D = temp2; 
	dlx[col3].D = temp3; 
	dlx[col4].D = temp4; 
	//差左右指標未串連
	dlx[temp1].R = temp2;
	dlx[temp2].R = temp3;
	dlx[temp3].R = temp4;
	dlx[temp4].R = temp1;
	dlx[temp1].L = temp4;
	dlx[temp2].L = temp1;
	dlx[temp3].L = temp2;
	dlx[temp4].L = temp3;
	return;
}
