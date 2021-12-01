#include<iostream>
using namespace std;

int num=0;
int p,q;
int tmpQP[3][3];
int cur[3][3];
const int depth=3;

void init()
{
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            cur[i][j] = 0;
        }
    }
}

void print()
{
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            cout<<cur[i][j]<<" ";
        }
        cout<<endl;
    }
}

void human_input()
{
    int location,row,column,check;
    do
    {
        cout<<"Enter the location of your choice (for example: 32, 3 row 2 column):";
        cin>>location;
        row = location/10;
        column = location%10;
        if(row>0 && row<4 &&column>0 && column<4 && cur[row-1][column-1]==0)
        {
            cur[row-1][column-1]=-1;
            check = 1;
        }
        else if(cur[row-1][column-1]!=0)
        {
            cout<<"Input Error!Already piece here!";
            check = -1;
        }
        else
        {
            cout<<"Input Error!Out of board range!";
            check = -1;
        }
    }while(check!=1);
}

int checkwin()
{
    for(int i=0;i<3;i++)
    {
        if(cur[i][0]==1 && cur[i][1]==1 && cur[i][2]==1)
        {
            return 1;
        }
        if(cur[i][0]==-1 && cur[i][1]==-1 && cur[i][2]==-1)
        {
            return -1;
        }
    }
    for(int i=0;i<3;i++)
    {
        if(cur[0][i]==1 && cur[1][i]==1 && cur[2][i]==1)
        {
            return 1;
        }
        if(cur[0][i]==-1 && cur[1][i]==-1 && cur[2][i]==-1)
        {
            return -1;
        }
    }
    if((cur[0][0]==1&&cur[1][1]==1&&cur[2][2]==1)||(cur[2][0]==1&&cur[1][1]==1&&cur[0][2]==1))
	{
		return 1;
	}
	if((cur[0][0]==-1&&cur[1][1]==-1&&cur[2][2]==-1)||(cur[2][0]==-1&&cur[1][1]==-1&&cur[0][2]==-1))
	{
		return -1;
	}
	return 0;
}

int value()
{
	p=0;
	q=0;
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			if(cur[i][j]==0)
			{
				tmpQP[i][j]=1;
			}
			else
			{
				tmpQP[i][j]=cur[i][j];
			}
		}
	}
 	for(int i=0;i<3;i++)
	{
		p+=(tmpQP[i][0]+tmpQP[i][1]+tmpQP[i][2])/3;
	}
	for(int i=0;i<3;i++)
	{
		p+=(tmpQP[0][i]+tmpQP[1][i]+tmpQP[2][i])/3;
	}

	p+=(tmpQP[0][0]+tmpQP[1][1]+tmpQP[2][2])/3;
	p+=(tmpQP[2][0]+tmpQP[1][1]+tmpQP[0][2])/3;

	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			if(cur[i][j]==0)
			{
				tmpQP[i][j]=-1;
			}
			else
			{
				tmpQP[i][j]=cur[i][j];
			}
		}
	}
	for(int i=0;i<3;i++)
	{
		q+=(tmpQP[i][0]+tmpQP[i][1]+tmpQP[i][2])/3;
	}
	for(int i=0;i<3;i++)
	{
		q+=(tmpQP[0][i]+tmpQP[1][i]+tmpQP[2][i])/3;
	}
	q+=(tmpQP[0][0]+tmpQP[1][1]+tmpQP[2][2])/3;
	q+=(tmpQP[2][0]+tmpQP[1][1]+tmpQP[0][2])/3;
	return p+q;
}

int cut(int &alpha,int dep,bool is_ai)
{
    if(dep==depth||dep+num==9)
    {
        return value();
    }
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
    for(int i=0;i<3 && !out;i++)
    {
        for(int j=0;j<3 && !out;j++)
        {
            if(cur[i][j]==0)
            {
                if(is_ai)
                {
                    cur[i][j]=-1;
                    if(checkwin()==-1)
					{
						temp=-10000;
					}
                    else
					{
                        temp=cut(beta,dep+1,!is_ai);
					}
                    if(temp<beta)
					{
                        beta=temp;
					}
                    if(beta<=alpha)
					{
						out=true;
                }
					}
                else
                {
                    cur[i][j]=1;
                    if(checkwin()==1)
					{
                        temp=10000;
					}
                    else
					{
                        temp=cut(beta,dep+1,!is_ai);
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
				cur[i][j]=0;
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

int main()
{
    int m=-10000, alpha=-10000,dep=1;
    int x_pos,y_pos;
    init();
    print();
    while(true)
    {
        human_input();
        print();
        cout<<endl;
        num++;
        value();
        if(q==0)
        {
            cout<<"The game draw!"<<endl;
            return 0;
        }
        if(checkwin()==-1)
        {
            cout<<"You win!Game over"<<endl;
            return 0;
        }
        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
                if(cur[i][j]==0)
                {
                    cur[i][j]=1;
                    cut(alpha,dep,1);
                    if(checkwin()==1)
                    {
                        cout<<"AI move in:"<<i+1<<","<<j+1<<endl;
                        print();
                        cout<<"AI win!game over"<<endl;
                        return 0;
                    }
                    if(alpha>m)
                    {
                        m=alpha;
                        x_pos=i;
                        y_pos=j;
                    }
                    alpha=-10000;
                    cur[i][j]=0;
                }
            }
        }
        cur[x_pos][y_pos]=1;
        alpha=-10000;
        m=-10000;
        dep=1;
        cout<<"AI move in:"<<x_pos+1<<","<<y_pos+1<<endl;
        print();
        cout<<endl;
        num++;
        value();
        if(q==0)
        {
            cout<<"The game draw!"<<endl;
            return 0;
        }
    }
    return 0;
}
