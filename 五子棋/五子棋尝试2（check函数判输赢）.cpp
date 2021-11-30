#include<iostream>
#include<vector>
#include"string.h"
using namespace std;

#define none_chess 0
#define black_chess 1
#define white_chess 2
//human执黑先手

//定义棋形
#define other_way 0
#define human_win 1 //10000000
#define ai_win 2 //-10000000

#define OTHER 0//0,其他棋型不考虑
#define WIN 1//100000,白赢
#define LOSE 2//-10000000
#define FLEX4 3//50000,白活4
#define flex4 4//-80000
#define BLOCK4 5//400
#define block4 6//-80000
#define FLEX3 7//400
#define flex3 8//-8000
#define BLOCK3 9//20
#define block3 10//-40
#define FLEX2 11//20
#define flex2 12//-40
#define BLOCK2 13//1
#define block2 14//-2
#define FLEX1 15//1
#define flex1 16//-2

int chess_board[15][15];
const int depth=4;
int check=0;
int tuple6type[4][4][4][4][4][4];

struct Evaluate
{
    unsigned long long score;
    int result;
    int keeper[8];
};
void print()
{

    for(int i=0;i<15;i++)
	{
		for(int j=0;j<15;j++)
        {
            cout<<chess_board[i][j]<<"  ";
        }
		cout<<endl;
	}
}

void human_input()
{
    int location,row,column,check_number;
    do
    {
        cout<<"Enter the location of your choice (for example: 32, 3 row 2 column):";
        cin>>location;
        if(location<=100)
        {
            row = location/10;
            column = location%10;
        }
        else
        {
            row = location/100;
            column = location%100;
        }
        if(row>0 && row<16 &&column>0 && column<16 && chess_board[row-1][column-1]==0)
        {
            chess_board[row-1][column-1]=1;
            check_number = 1;
        }
        else if(chess_board[row-1][column-1]!=0)
        {
            cout<<"Input Error!Already piece here!";
            check_number = -1;
        }
        else
        {
            cout<<"Input Error!Out of board range!";
            check_number = -1;
        }
    }while(check_number!=1);
}

void initial_board()
{
	for(int i=0;i<15;i++)
    {
        for(int j=0;j<15;j++)
        {
            chess_board[i][j]=0;
        }
    }

}
void initial_tuple6type()
{
    memset(tuple6type,0,sizeof (tuple6type));

    tuple6type[2][2][2][2][2][2]=WIN;
    tuple6type[2][2][2][2][2][0]=WIN;
    tuple6type[0][2][2][2][2][2]=WIN;
    tuple6type[2][2][2][2][2][1]=WIN;
    tuple6type[1][2][2][2][2][2]=WIN;
    tuple6type[3][2][2][2][2][2]=WIN;
    tuple6type[2][2][2][2][2][3]=WIN;

    tuple6type[1][1][1][1][1][1]=LOSE;
    tuple6type[1][1][1][1][1][0]=LOSE;
    tuple6type[0][1][1][1][1][1]=LOSE;
    tuple6type[1][1][1][1][1][2]=LOSE;
    tuple6type[2][1][1][1][1][1]=LOSE;
    tuple6type[3][1][1][1][1][1]=LOSE;
    tuple6type[1][1][1][1][1][3]=LOSE;

    tuple6type[0][2][2][2][2][0]=FLEX4;

    tuple6type[0][1][1][1][1][0]=flex4;

    tuple6type[0][2][2][2][0][0]=FLEX3;
    tuple6type[0][0][2][2][2][0]=FLEX3;
    tuple6type[0][2][0][2][2][0]=FLEX3;
    tuple6type[0][2][2][0][2][0]=FLEX3;

    tuple6type[0][1][1][1][0][0]=flex3;
    tuple6type[0][0][1][1][1][0]=flex3;
    tuple6type[0][1][0][1][1][0]=flex3;
    tuple6type[0][1][1][0][1][0]=flex3;

    tuple6type[0][2][2][0][0][0]=FLEX2;
    tuple6type[0][2][0][2][0][0]=FLEX2;
    tuple6type[0][2][0][0][2][0]=FLEX2;
    tuple6type[0][0][2][2][0][0]=FLEX2;
    tuple6type[0][0][2][0][2][0]=FLEX2;
    tuple6type[0][0][0][2][2][0]=FLEX2;

    tuple6type[0][1][1][0][0][0]=flex2;
    tuple6type[0][1][0][1][0][0]=flex2;
    tuple6type[0][1][0][0][1][0]=flex2;
    tuple6type[0][0][1][1][0][0]=flex2;
    tuple6type[0][0][1][0][1][0]=flex2;
    tuple6type[0][0][0][1][1][0]=flex2;

    tuple6type[0][2][0][0][0][0]=FLEX1;
    tuple6type[0][0][2][0][0][0]=FLEX1;
    tuple6type[0][0][0][2][0][0]=FLEX1;
    tuple6type[0][0][0][0][2][0]=FLEX1;

    tuple6type[0][1][0][0][0][0]=flex1;
    tuple6type[0][0][1][0][0][0]=flex1;
    tuple6type[0][0][0][1][0][0]=flex1;
    tuple6type[0][0][0][0][1][0]=flex1;

    int p1,p2,p3,p4,p5,p6,x,y,ix,iy;
    for(p1=0;p1<4;++p1){
        for(p2=0;p2<3;++p2){
            for(p3=0;p3<3;++p3){
                for(p4=0;p4<3;++p4){
                    for(p5=0;p5<3;++p5){
                        for(p6=0;p6<4;++p6){
                            x=y=ix=iy=0;

                            if(p1==1)x++;
                            else if(p1==2)y++;

                            if(p2==1){x++;ix++;}
                            else if(p2==2){y++;iy++;}

                            if(p3==1){x++;ix++;}
                            else if(p3==2){y++;iy++;}

                            if(p4==1){x++;ix++;}
                            else if(p4==2){y++;iy++;}

                            if(p5==1){x++;ix++;}
                            else if(p5==2){y++;iy++;}

                            if(p6==1)ix++;
                            else if(p6==2)iy++;

                            if(p1==3||p6==3){
                                if(p1==3&&p6!=3){

                                    if(ix==0&&iy==4){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=BLOCK4;
                                    }

                                    if(ix==4&&iy==0){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=block4;
                                    }

                                    if(ix==0&&iy==3){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=BLOCK3;
                                    }

                                    if(ix==3&&iy==0){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=block3;
                                    }

                                    if(ix==0&&iy==2){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=BLOCK2;
                                    }

                                    if(ix==2&&iy==0){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=block2;
                                    }
                                }else if(p6==3&&p1!=3){

                                    if(x==0&&y==4){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=BLOCK4;
                                    }

                                    if(x==4&&y==0){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=block4;
                                    }

                                    if(x==3&&y==0){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=BLOCK3;
                                    }

                                    if(x==0&&y==3){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=block3;
                                    }

                                    if(x==2&&y==0){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=BLOCK2;
                                    }

                                    if(x==0&&y==2){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=block2;
                                    }
                                }
                            }else{

                                if((x==0&&y==4)||(ix==0&&iy==4)){
                                    if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                        tuple6type[p1][p2][p3][p4][p5][p6]=BLOCK4;
                                }

                                if((x==4&&y==0)||(ix==4&&iy==0)){
                                    if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                        tuple6type[p1][p2][p3][p4][p5][p6]=block4;
                                }

                                if((x==0&&y==3)||(ix==0&&iy==3)){
                                    if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                        tuple6type[p1][p2][p3][p4][p5][p6]=BLOCK3;
                                }

                                if((x==3&&y==0)||(ix==3&&iy==0)){
                                    if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                        tuple6type[p1][p2][p3][p4][p5][p6]=block3;
                                }

                                if((x==0&&y==2)||(ix==0&&iy==2)){
                                    if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                        tuple6type[p1][p2][p3][p4][p5][p6]=BLOCK2;
                                }

                                if((x==2&&y==0)||(ix==2&&iy==0)){
                                    if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                        tuple6type[p1][p2][p3][p4][p5][p6]=block2;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


//1.实现评估函数
int evaluation(int* check)
{
    int weight[17]={0,1000000,-10000000,50000,-100000,400,-100000,400,-8000,20,-50,20,1,-3,1,-3};
    int count_type[4][17],type;
    memset(count_type,0,sizeof(count_type));
    int value[17];
    unsigned long long score_now=0;

    int virtual_board[17][17];
    for(int i=0;i<17;++i)
    {
        virtual_board[i][0]=3;
    }
    for(int i=0;i<17;++i)
    {
        virtual_board[i][16]=3;
    }
    for(int i=0;i<17;++i)
    {
        virtual_board[0][i]=3;
    }
    for(int i=0;i<17;++i)
    {
        virtual_board[16][i]=3;
    }
    for(int i=0;i<15;++i)
    {
        for(int j=0;j<15;++j)
        {
            virtual_board[i+1][j+1]=chess_board[i][j];
        }
    }

    for(int i=1;i<=15;++i){
        for(int j=0;j<12;++j){
            type=tuple6type[virtual_board[i][j]][virtual_board[i][j+1]][virtual_board[i][j+2]][virtual_board[i][j+3]][virtual_board[i][j+4]][virtual_board[i][j+5]];
            count_type[0][type]++;
        }
    }

    for(int j=1;j<=15;++j){
        for(int i=0;i<12;++i){
            type=tuple6type[virtual_board[i][j]][virtual_board[i+1][j]][virtual_board[i+2][j]][virtual_board[i+3][j]][virtual_board[i+4][j]][virtual_board[i+5][j]];
            count_type[1][type]++;
        }
    }

    for(int i=0;i<12;++i){
        for(int j=0;j<12;++j){
            type=tuple6type[virtual_board[i][j]][virtual_board[i+1][j+1]][virtual_board[i+2][j+2]][virtual_board[i+3][j+3]][virtual_board[i+4][j+4]][virtual_board[i+5][j+5]];
            count_type[2][type]++;
        }
    }

    for(int i=0;i<12;++i){
        for(int j=5;j<17;++j){
            type=tuple6type[virtual_board[i][j]][virtual_board[i+1][j-1]][virtual_board[i+2][j-2]][virtual_board[i+3][j-3]][virtual_board[i+4][j-4]][virtual_board[i+5][j-5]];
            count_type[3][type]++;
        }
    }


    for(int i=1;i<17;i++)
    {
        score_now +=(count_type[0][i]+count_type[1][i]+count_type[2][i]+count_type[3][i])*(weight[i]);
        int counter=count_type[0][i]+count_type[1][i]+count_type[2][i]+count_type[3][i];
        if(i==WIN)
        {
            value[WIN]=counter;
        }
        else if(i==LOSE)
        {
            value[LOSE]=counter;
        }
    }

    if(value[WIN]>0)
    {
        *check=1;
    }
    else if(value[LOSE]>0)
    {
        *check=-1;
    }

    return score_now;
}
//2.实现剪枝函数（先实现遍历棋盘再试着优化局部搜索）（最佳落子）
int cut(int &alpha,int dep,bool is_ai,int *check)
{
    if(dep==depth)
        return evaluation(check);
    int beta,temp;
    bool out=false;
    if(is_ai)
	{
		beta=10000;
	}
    else
	{
        beta=-10000;
	}
    for(int i=0;i<15&&!out;i++)
    {
        for(int j=0;j<15&&!out;j++)
        {
            if(chess_board[i][j]==0)
            {
                if(is_ai)
                {
                    chess_board[i][j]=1;
                    if(*check==1)
					{
						temp=-100000000;
					}
                    else
					{
                        temp=cut(beta,dep+1,!is_ai,check);
                    if(temp<beta)
					{
                        beta=temp;
					}
                    if(beta<=alpha)
					{
						out=true;
                    }
                else
                    chess_board[i][j]=2;
                    if(*check==-1)
					{
                        temp=100000000;
					}
                    else
					{
                        temp=cut(beta,dep+1,!is_ai,check);
					}
                    if(temp>beta)
					{
                        beta=temp;
					}
                    if(beta>=alpha)
					{
                        out=true;
					}
                }
				chess_board[i][j]=0;
            }
        }
    }
    if(is_ai)
    {
		if(beta>alpha)
		{
            alpha=beta;
		}
    }
    else
    {
        if(beta<alpha)
		{
            alpha=beta;
		}
    }
    return beta;
    }
}

int cut2()
{

}

int main()
{
    int m=-10000,alpha=-10000,dep=1;
	int x_pos,y_pos;
    initial_board();
    initial_tuple6type();
    print();
    while(true)
    {
        human_input();
        evaluation(&check);
        if(check==1)
        {
            cout<<"You Win! GAME OVER."<<endl;
            return 0;
        }
        for(int i=0;i<15;i++)
		{
			for(int j=0;j<15;j++)
			{
				if(chess_board[i][j]==0)
				{
					chess_board[i][j]=2;
					cut(alpha,dep,1,&check);
                    evaluation(&check);
					if(check==-1)
					{
						cout<<"AI locate the chess in:"<<i+1<<j+1<<endl;
						print();
						cout<<"AI WIN! GAME OVER."<<endl;
						return 0;
					}
					chess_board[i][j]=0;
					if(alpha>m)
					{
						m=alpha;
						x_pos=i;y_pos=j;
					}
					alpha=-10000;

				}
			}
		}
		chess_board[x_pos][y_pos]=2;
		alpha=-10000;
		m=-10000;
		dep=1;
		cout<<"AI locate the chess in:"<<x_pos+1<<y_pos+1<<endl;
		print();
    }
    return 0;
}

