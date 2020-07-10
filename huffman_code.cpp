////test///
#pragma warning(disable : 4786)
#pragma warning(disable:4996)
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <map>
using namespace std;
map<char, string> mapHuffmanTree;
typedef struct {
	char data;
	string bitstr;
	double weight;
	int parent, lchild, rchild;
}HtNode, * HuffmanTree;
typedef struct {
	string ostr;
	string bitstr;
}HcNode, * HtTable;
void select(HuffmanTree HT, int cur, int& min1, int& min2)
{
	min1 = min2 = 0;
	for (int i = 1; i < cur; ++i)
	{
		if (HT[i].parent != 0)
		{
			continue;
		}
		if (min1 == 0)
		{
			min1 = min2 = i;
		}
		else {
			if (HT[i].weight <= HT[min1].weight)
			{
				min2 = min1;
				min1 = i;
			}
			else if (HT[i].weight < HT[min2].weight)
			{
				min2 = i;
			}
			else if (HT[i].weight > HT[min2].weight)
			{
				if (min1 == min2)
				{
					min2 = i;
				}
			}
		}
	}
}

void CreateHuffmanTree(HuffmanTree& HT, char data[], double weight[], int n)
{
	if (n < 1)
	{
		return;
	}
	int i;
	int m = 2 * n - 1;
	HT = new HtNode[m + 1];

	for (i = 1; i <= m; ++i)
	{
		HT[i].parent = 0;
		HT[i].lchild = HT[i].rchild = 0;

	}
	for (i = 1; i <= n; ++i)
	{
		HT[i].weight = weight[i - 1];
		HT[i].data = data[i - 1];
	}
	for (i = n + 1; i <= m; ++i)
	{
		int min1, min2;
		select(HT, i, min1, min2);
		HT[i].weight = HT[min1].weight + HT[min2].weight;
		HT[i].lchild = min1;
		HT[i].rchild = min2;
		HT[min1].parent = HT[min2].parent = i;
	}

}

typedef char** HuffmanCodeTable;
void CreateHuffmanCode(HuffmanTree HT, HuffmanCodeTable& HC, int n)
{
	HC = new char* [n + 1];
	char* cd = new char[n];
	cd[n - 1] = '\0';
	for (int i = 1; i <= n; ++i)
	{
		int start = n - 1;
		int c = i;
		int f = HT[i].parent;
		while (f != 0)
		{
			start--;
			if (HT[f].lchild == c)
			{
				cd[start] = '0';
			}
			else { cd[start] = '1'; }
			c = f;
			f = HT[f].parent;
		}
		HC[i] = new char[n - start];
		strcpy(HC[i], &cd[start]);
		HT[i].bitstr = HC[i];
		mapHuffmanTree.insert(pair<char, string>(HT[i].data, HC[i]));
	}

	delete[] cd;
}

int main()
{
	double weights[] = { 0.2342,0.0766,0.0049,0.0207,0.0288,0.1212,
		0.0199,0.0109,0.0438,0.0447,0.0013,
		0.0042,0.0249,0.0159,0.0527,0.0707,
		0.0109,0.0012,0.0587,0.0427,0.0955,
		0.0069,0.0002,0.0059,0.0017,0.0019,0.0008 };
	char data[] = { ' ','a','b','c','d','e','f','g','h','i','j','k'
					,'l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };

	//{5,29,7,8,14,23,3,11};
	const int LENGH = 27;

	HuffmanTree HT;
	int i;
	CreateHuffmanTree(HT, data, weights, LENGH);
	for (i = 1; i <= 2 * LENGH - 1; ++i)
	{
		HtNode node = HT[i];
		cout << i << "\t|"
			<< node.weight << "\t|"
			<< node.parent << "\t|"
			<< node.lchild << "\t|"
			<< node.rchild << "\t|" << endl;
	}

	///
	HuffmanCodeTable hct;
	CreateHuffmanCode(HT, hct, LENGH);
	map<char, string>::iterator iter1 = mapHuffmanTree.begin();
	for (i = 1; i <= LENGH; ++i)
	{
		cout << i << ":" << weights[i - 1] << "->" <<iter1->first<<"->"<< iter1->second.c_str() << endl;
		iter1++;
	}

	char text[100];
	cout << "================Input Your Text=================" << endl;
	//gets(text);
	cin.getline(text, 100);
	//char text[]="abcd";
	int text_len = strlen(text);
	char* encoded_text = new char[text_len * LENGH];
	int start_ine = 0;
	for (i = 0; i < text_len; ++i)
	{
		// 		char c = text[i];
		// 		//int index = c -'a'+1;
		// 		int index=c -'a'+2;
		// 		char *huffman_code;
		// 		if (index==-63)
		// 		{
		// 			huffman_code = hct[1];
		// 		}
		// 		else
		// 		{
		// 			huffman_code = hct[index];
		// 		}
		// 		/*char *huffman_code = const_cast<char *>(st.c_str()) ;*/
		map<char, string>::iterator iter;
		iter = mapHuffmanTree.find(text[i]);
		if (iter != mapHuffmanTree.end())
		{
			char* huffman_code = const_cast<char*>(iter->second.c_str());
			strcpy(encoded_text + start_ine, huffman_code);
			start_ine += strlen(huffman_code);
		}
		else
		{
			cout << "Do not Find HuffmanCode!" << endl;
		}

	}
	cout << "encoded:" << encoded_text << endl;;

	int root_index = 1;
	//while (HT[root_index].parent != 0)
	//{
	//	root_index++;
	//}
	root_index = 2* LENGH-1;
	char decoded_text[100];
	int cur_dec = 0;
	int encode_len = strlen(encoded_text);
	int j = 0;
	while (j < encode_len)
	{
		int p = root_index;
		while (HT[p].lchild != 0 || HT[p].rchild != 0)
		{
			char code = encoded_text[j++];
			if (code == '0')
			{
				p = HT[p].lchild;
			}
			else
			{
				p = HT[p].rchild;
			}
		}
		// 		if (p==1)
		// 		{
		// 			decoded_text[cur_dec]=' ';
		// 		}
		// 		else
		// 		{
		// 			decoded_text[cur_dec] = 'a'+p-2;
		// 		}
		decoded_text[cur_dec] = HT[p].data;
		cur_dec++;
	}
	decoded_text[cur_dec] = '\0';
	cout << "decoded:" << decoded_text << endl;
	//delete
	mapHuffmanTree.clear();
	for (i = 1; i <= 27; ++i)
	{
		delete[] hct[i];
	}
	delete[] hct;
	delete[] HT;

	return 0;
}
