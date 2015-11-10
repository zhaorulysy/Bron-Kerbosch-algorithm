#include<iostream>
using namespace std;

#define SET_NUM 25

int  pre[1050];
bool t[1050];               //t 用于标记独立块的根结点
int init_data[SET_NUM][SET_NUM];

const int set_num = 6;
struct sets_info
{
	int size;
	int sets_ele[SET_NUM];
};
int all_sets_index = 0;

struct sets_info sets_arr[SET_NUM];
struct sets_info *connect_eles;

int find(int x)
{
	int r=x;
	while(r!=pre[r])
		r=pre[r];
	
	int i=x,j;
	while(pre[i]!=r)
	{
		j=pre[i];
		pre[i]=r;
		i=j;
	}
	return r;
}

void join(int x,int y)
{
	int fx=find(x),fy=find(y);
	if(fx!=fy)
	{
		pre[fy]=fx;
	}
} 

int set_find_ele(struct sets_info * arr_info,int ele)
{
	int low = 0,hight = arr_info->size - 1;
	
	while(low <= hight)
	{
		int middle = (low + hight) / 2;
		if(arr_info->sets_ele[middle] < ele)
		{
			low = middle + 1;
		}
		else if(arr_info->sets_ele[middle] > ele)
		{
			hight = middle - 1;
		}
		else
		{
			return middle;
		}
	}
	return -1;
}

void set_union_ele(struct sets_info * arr_info,int ele)
{
	if(arr_info->size == 0)
	{
		arr_info->size = 1;
		arr_info->sets_ele[0] = ele;
		return;
	}
	int i;
	int index = set_find_ele(arr_info,ele);
	if(index == -1)
	{
		
		for(i = arr_info->size -1;i >=0;i--)
		{
			if(arr_info->sets_ele[i] > ele)
			{
				arr_info->sets_ele[i+1] = arr_info->sets_ele[i];
			}
			else
			{
				arr_info->sets_ele[i+1] = ele;
				arr_info->size = arr_info->size +1;
				return;
			}	
		}
	}
}

void set_div_ele(struct sets_info * arr_info,int ele)
{
	int i;
	int index = set_find_ele(arr_info,ele);
	if(index == -1) return;
	else
	{
		for(i = index;i<arr_info->size-1;i++)
		{
			arr_info->sets_ele[i] = arr_info->sets_ele[i+1];
		}
		arr_info->size = arr_info->size - 1;
		return;
	}
}
void copy_sets_info(struct sets_info * source,struct sets_info * destination)
{
	int i;
	for(i = 0;i <source->size;i++)
	{
		destination->sets_ele[i] = source->sets_ele[i];
	}
	destination->size = source->size;
}

struct sets_info * set_union_ele_new(struct sets_info * arr_info,int ele)
{
	struct sets_info * tmp = (sets_info *)malloc(sizeof(sets_info));
	copy_sets_info(arr_info,tmp);
	int index = set_find_ele(tmp,ele);
	if(index == -1)
	{
		set_union_ele(tmp,ele);
	}
	return tmp;
}

struct sets_info * set_div_set(struct sets_info * original_sets,struct sets_info div_sets)
{
	struct sets_info * tmp = (sets_info *)malloc(sizeof(sets_info));
	int i = 0,j = 0,tmp_index = 0;
	while(i < original_sets->size && j < div_sets.size)
	{
		if(original_sets->sets_ele[i] < div_sets.sets_ele[j])
		{
			tmp->sets_ele[tmp_index] = original_sets->sets_ele[i];
			i++;
			tmp_index++;
		}
		else if(original_sets->sets_ele[i] > div_sets.sets_ele[j]){
			j++;
		}
		else{
			i++;j++;
		}
	}
	while(i < original_sets->size)
	{
		tmp->sets_ele[tmp_index] = original_sets->sets_ele[i];
		i++;
		tmp_index++;
	}
	tmp->size = tmp_index;
	return tmp;
}

struct sets_info * two_sets_union(struct sets_info *first,struct sets_info *second)
{
	struct sets_info * tmp = (sets_info *)malloc(sizeof(sets_info));
	int first_num = 0,second_num = 0,tmp_mun = 0;
	while(first_num < first->size && second_num < second->size)
	{
		if(first->sets_ele[first_num] < second->sets_ele[second_num])
		{
			tmp->sets_ele[tmp_mun++] = first->sets_ele[first_num];
			first_num++;
		}
		else
		{
			tmp->sets_ele[tmp_mun++] = second->sets_ele[second_num];
			second_num++;
		}
	}
	while(first_num < first->size)
	{
		tmp->sets_ele[tmp_mun++] = first->sets_ele[first_num];
		first_num++;
	}
	while(second_num < second->size)
	{
		tmp->sets_ele[tmp_mun++] = second->sets_ele[second_num];
		second_num++;
	}
	tmp->size = tmp_mun;
	return tmp;
}

struct sets_info * two_sets_intersection(struct sets_info *first,struct sets_info second)
{
	struct sets_info * tmp = (sets_info *)malloc(sizeof(sets_info));
	int first_num = 0,second_num = 0,tmp_mun = 0;
	while(first_num < first->size && second_num < second.size)
	{
		if(first->sets_ele[first_num] < second.sets_ele[second_num])
		{
			first_num++;
		}
		else if(first->sets_ele[first_num] > second.sets_ele[second_num])
		{
			second_num++;
		}
		else
		{
			tmp->sets_ele[tmp_mun++] = first->sets_ele[first_num];
			first_num++;second_num++;
		}
		
	}
	tmp->size = tmp_mun;
	return tmp;
}

void print_arr(int *print_arr,int arr_len)
{
	int i;
	cout<<"start print_arr  "<<arr_len<<endl;
	for(i = 0;i<arr_len;i++)
	{
		cout<<print_arr[i]<<" ";
	}
	cout<<endl<<"end print_arr"<<endl;
} 

void Bron_Kerbosch(struct sets_info * r,struct sets_info * p,struct sets_info * x)
{
	if(p->size == 0 && x->size == 0)
	{
		cout<< "&&&&"<<r->size<<endl;
		print_arr(r->sets_ele,r->size);
		return;
	}

	int i;
	while(p->size > 0)
	{
		i = 0;
		struct sets_info * r_v;
		struct sets_info * p__nv;
		struct sets_info * x__nv;
		r_v = set_union_ele_new(r,p->sets_ele[i]);
		p__nv = two_sets_intersection(p,connect_eles[p->sets_ele[i]]);
		x__nv = two_sets_intersection(x,connect_eles[p->sets_ele[i]]);
		Bron_Kerbosch(r_v,p__nv,x__nv);
		set_div_ele(p,p->sets_ele[i]);
		set_union_ele(x,p->sets_ele[i]);	
	}

}
void insert_setsele (int *sets_ele,int sets_len)
{

	int i,j;
	//bool is_has_repeat = false;
	
	//if (!is_has_repeat)
	{
		sets_arr[all_sets_index].size = sets_len;
		for(i = 0;i<sets_len;i++)
		{
			sets_arr[all_sets_index].sets_ele[i] = sets_ele[i];
		}
		
		all_sets_index++;

	}
}


int main()
{
	freopen("11.txt","r",stdin);
 	freopen("2.txt","w",stdout);
 	
 	
 	int i ,j,k;
 	int get_sets[SET_NUM];
 	connect_eles = (sets_info *)malloc(sizeof(sets_info)*set_num);
 	for (i = 0;i<SET_NUM;i++)
 	{
		for(j= 0;j<SET_NUM;j++)
		{
			init_data[i][j] = 0;
		}
		get_sets[i] = -1;
	}
 	for (i = 0;i<set_num;i++)
 	{
		for(j= 0;j<set_num;j++)
		{
			cin>>init_data[i][j];
		}
	}
	for(i = 0;i<set_num;i++)
		pre[i] = i;
		
	for (i = 0;i<set_num;i++)
 	{
		for(j= i;j<set_num;j++)
		{
			if(init_data[i][j] == 1)
			{
				join(i,j);
			}
		}
		int num = 0;
		for(k = 0;k < set_num;k++)
		{
			if(init_data[i][k] == 1 && i != k)
			{
				connect_eles[i].sets_ele[num] = k;
				num++;
			}
		}
		connect_eles[i].size = num;
		cout<<"iii  "<<i<<endl;
		print_arr(connect_eles[i].sets_ele,connect_eles[i].size);
	}
	
	
	for(i = 0;i<set_num;i++)
	{
		t[find(i)] = true;
	}
	for(i = 0;i < set_num;i++)
	{
		if (t[i] == true)
		{
			int k=0;
			for (j = 0;j<SET_NUM;j++)
				get_sets[j] = -1;
			for(j = 0;j<set_num;j++)
			{
				if(find(j)==i)
				{
					get_sets[k++] = j;
				}
			}
			
			// cout<<"@@@@"<<endl;
			
			// cout<<endl;
			
			if(k == 1)
				insert_setsele(get_sets,1);
			else if(k == 2)
				insert_setsele(get_sets,2);
			else	
			{
				cout<<"----------main:"<<k<<endl;
				struct sets_info R,P,X;
				int h ;
				for (h = 0;h < k;h++)
				{
					P.sets_ele[h] = get_sets[h];
				}
				P.size = k;
				R.size = 0;
				X.size = 0;
				Bron_Kerbosch(&R,&P,&X);
			}
		}
		
	}
	
//	for (i = 0;i<all_sets_index;i++)
// 	{
//		cout<<i<<"***"<<sets_arr[i].size<<endl;
//		if (sets_arr[i].size == 0) continue;
//		for(j = 0;j<sets_arr[i].size;j++)
//		{
//			cout<<sets_arr[i].sets_ele[j]<<" ";
//		}
//		cout<<endl;
//	}
	
	return 0;
}//dellaserss
