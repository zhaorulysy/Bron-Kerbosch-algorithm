#include<iostream>
using namespace std;

#define SET_NUM 25

int init_data[SET_NUM][SET_NUM];

const int set_num = 7;
struct sets_info
{
	int size;
	int sets_ele[SET_NUM];
};
int all_sets_index = 0;

struct sets_info sets_arr[SET_NUM];
struct sets_info *connect_eles;

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
				if(i == 0)//notes
				{
					arr_info->size = arr_info->size +1;
					arr_info->sets_ele[i] = ele;
				}
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

struct sets_info * set_div_set(struct sets_info * original_sets,struct sets_info *div_sets)
{
	struct sets_info * tmp = (sets_info *)malloc(sizeof(sets_info));
	int i = 0,j = 0,tmp_index = 0;
	while(i < original_sets->size && j < div_sets->size)
	{
		if(original_sets->sets_ele[i] < div_sets->sets_ele[j])
		{
			tmp->sets_ele[tmp_index] = original_sets->sets_ele[i];
			i++;
			tmp_index++;
		}
		else if(original_sets->sets_ele[i] > div_sets->sets_ele[j]){
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

struct sets_info * two_sets_intersection(struct sets_info *first,struct sets_info *second)
{
	struct sets_info * tmp = (sets_info *)malloc(sizeof(sets_info));
	int first_num = 0,second_num = 0,tmp_mun = 0;
	while(first_num < first->size && second_num < second->size)
	{
		if(first->sets_ele[first_num] < second->sets_ele[second_num])
		{
			first_num++;
		}
		else if(first->sets_ele[first_num] > second->sets_ele[second_num])
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
	struct sets_info * tmp = two_sets_union(p,x);
	struct sets_info * tmp2 = set_div_set(p,&connect_eles[tmp->sets_ele[0]]);
	free(tmp);tmp = NULL;	

	int i;
	for(i = 0;i < tmp2->size;i++)
	{
		int v = tmp2->sets_ele[i];
		
		struct sets_info * r_v;
		struct sets_info * p__nv;
		struct sets_info * x__nv;
		r_v = set_union_ele_new(r,v);		
		p__nv = two_sets_intersection(p,&connect_eles[v]);
		x__nv = two_sets_intersection(x,&connect_eles[v]);
		Bron_Kerbosch(r_v,p__nv,x__nv);
		set_div_ele(p,v);
		set_union_ele(x,v);	
		free(r_v);r_v = NULL;
		free(p__nv);p__nv = NULL;
		free(x__nv);x__nv = NULL;		
	}
	free(tmp2);tmp2 = NULL;
}
void insert_setsele (int *sets_ele,int sets_len)
{

	int i,j;
	bool is_has_repeat = false;
	
	if (!is_has_repeat)
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
	freopen("111.txt","r",stdin);
 	freopen("222.txt","w",stdout);
 	
 	
 	int i,j,k;
 	
 	connect_eles = (sets_info *)malloc(sizeof(sets_info)*set_num);
 	
 	int num = 0;
	for (i = 0;i<set_num;i++)
 	{
		cin>>num;
		for(k = 0;k < num;k++)
		{
			cin>>connect_eles[i].sets_ele[k];
		}
		connect_eles[i].size = num;
		
	}
//	for(i = 0;i<set_num;i++)
//		print_arr(connect_eles[i].sets_ele,connect_eles[i].size);
	
	cout<<"----------main:"<<k<<endl;
	struct sets_info R,P,X;
	int h ;
	for (h = 0;h < set_num;h++)
	{
		P.sets_ele[h] = h;
	}
	P.size = set_num;
	R.size = 0;
	X.size = 0;
	Bron_Kerbosch(&R,&P,&X);
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
