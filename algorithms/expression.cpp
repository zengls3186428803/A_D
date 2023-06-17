#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<stack>

#define MAX_N 100000

enum Lex_type{
	LEFT_BRACKET=0, RIGHT_BRACKET, PLUS, MINUS, MULTIPLY, DIVIDE, POWER, IDENTIFIER, CONST
};
char lextype[15][20] = {
	"LEFT_BRACKET",
	"RIGHT_BRACKET",
	"PLUS",
	"MINUS",
	"MULTIPLY",
	"DIVIDE",
	"POWER",
	"IDENTIFIER",
	"CONST"
};
typedef enum Lex_type Lex_type;

struct Lex{
	int type;
	char val[20];
};
typedef struct Lex Lex;

Lex lex_stream[10000];
int idx = 1;

void str_copy(char *src, int start, int end, char *dest) {
	int i = 0, len = end - start + 1;
	for(i = 0; i < len; ++i) {
		dest[i] = src[start + i];
	}
	dest[len] = 0;
	return;
}

void lexical_analyze(char *s, int len) {
	int state=-1;
	int start=0, end=0;
	int i = 0;
	while(1) {
		if(i == len) {
			if(state == IDENTIFIER || state == CONST) {
				lex_stream[++idx].type = state;  end = i - 1; str_copy(s, start, end, lex_stream[idx].val);
			}
			break;
		} else if(state == -1) {
			start = i;
			if(s[i] >= 'a' && s[i] <= 'z' || s[i] >= 'A' && s[i] <= 'Z') {
				state = IDENTIFIER; 
			} else if(s[i] >= '0' && s[i] <= '9') {
				state = CONST;
			} else if(s[i] == '(') {
				lex_stream[++idx].type = LEFT_BRACKET;  end = start; str_copy(s, start, end, lex_stream[idx].val);
			} else if(s[i] == ')') {
				lex_stream[++idx].type = RIGHT_BRACKET; end = start; str_copy(s, start, end, lex_stream[idx].val);
			} else if(s[i] == '+') {
				lex_stream[++idx].type = PLUS; end = start; str_copy(s, start, end, lex_stream[idx].val);
			} else if(s[i] == '-') {
				lex_stream[++idx].type = MINUS; end = start; str_copy(s, start, end, lex_stream[idx].val);
			} else if(s[i] == '*') {
				lex_stream[++idx].type = MULTIPLY;  end = start; str_copy(s, start, end, lex_stream[idx].val);
			} else if(s[i] == '/') {
				lex_stream[++idx].type = DIVIDE; end = start; str_copy(s, start, end, lex_stream[idx].val);
			} else if(s[i] == '^') {
				lex_stream[++idx].type = POWER; end = start; str_copy(s, start, end, lex_stream[idx].val);
			} else {
				printf("error 1: [%c]:[%d] is not define\n",s[i],i);
				return;
			}
			++i;
		} else if(state == IDENTIFIER) {
			if(s[i] >= 'a' && s[i] <= 'z' || s[i] >= 'A' && s[i] <= 'Z' || s[i] >= '0' && s[i] <= '9') {
				++i;
				//nothing to do
			} else {
				lex_stream[++idx].type = IDENTIFIER; end = i - 1; str_copy(s, start, end, lex_stream[idx].val);
				state = -1;
			}
		} else if(state == CONST) {
			if(s[i] >= '0' && s[i] <= '9') {
				++i;
				//nothing to do
			} else {
				lex_stream[++idx].type = CONST; end = i - 1; str_copy(s, start, end, lex_stream[idx].val);
				state = -1;
			}
		} else {
			printf("error 2: state is not define");
			return;
		}
	}
}

void print_lex_list() {
	printf("printf_lex_list,cnt=%d\n",idx-1);
	int i;
	for(i = 2; i <= idx; ++i) {
		printf("%-15s %-10s\n", lextype[lex_stream[i].type],lex_stream[i].val);
	}
}

//=============================================================================

using namespace std;
int superior_table[30][30];

void init_superior_table() {
	superior_table[PLUS][PLUS] = 1; superior_table[PLUS][MINUS] = 1; superior_table[PLUS][MULTIPLY] = 0; superior_table[PLUS][DIVIDE] = 0; superior_table[PLUS][POWER] = 0;
	superior_table[MINUS][PLUS] = 1; superior_table[MINUS][MINUS] = 1; superior_table[MINUS][MULTIPLY] = 0; superior_table[MINUS][DIVIDE] = 0; superior_table[MINUS][POWER] = 0;
	superior_table[MULTIPLY][PLUS] = 1; superior_table[MULTIPLY][MINUS] = 1; superior_table[MULTIPLY][MULTIPLY] = 1; superior_table[MULTIPLY][DIVIDE] = 1; superior_table[MULTIPLY][POWER] = 0;
	superior_table[DIVIDE][PLUS] = 1; superior_table[DIVIDE][MINUS] = 1; superior_table[DIVIDE][MULTIPLY] = 1; superior_table[DIVIDE][DIVIDE] = 1; superior_table[DIVIDE][POWER] = 0;
	superior_table[POWER][PLUS] = 1; superior_table[POWER][MINUS] = 1; superior_table[POWER][MULTIPLY] = 1; superior_table[POWER][DIVIDE] = 1; superior_table[POWER][POWER] = 0;
}

int sup(int x, int y) { // return x > y
	return superior_table[x][y];
}

struct Syn{ //syntax or syntactic
	struct Syn *par;
	struct Syn *lson, *rson;
	Lex *lex;
};
typedef struct Syn Syn;
Syn syn_pool[MAX_N];
int idx_syn;

stack<Syn *> op_st,nu_st;
void insert(Syn *l, Syn *r, Syn *op) {//l,r,op is index of syn; return index of root;
	op->lson = l;
	op->rson = r;
	l->par = op;
	r->par = op;
}

Syn *generate_syntax_node(Lex *lex) {
	syn_pool[++idx_syn].lex = lex;
	syn_pool[idx_syn].par = 0;
	syn_pool[idx_syn].lson = 0;
	syn_pool[idx_syn].rson = 0;
	return &syn_pool[idx_syn];
}

void syntactic_analyze(Lex *lex_stream) {
	//最外层补一个括号,方便得到根节点.
	lex_stream[1].type = LEFT_BRACKET;
	lex_stream[++idx].type = RIGHT_BRACKET;

	int i = 1;
	while(1) {
		if(i>idx) break;
		int type = lex_stream[i].type;
		if(type == IDENTIFIER || type == CONST) {
			printf("type is ID or CONST\n");
			Syn *syn = generate_syntax_node(&lex_stream[i]);
			nu_st.push(syn);
			++i;
		} else {
			if(type == RIGHT_BRACKET) {
				while(1) {
					Syn *syn = op_st.top();
					op_st.pop();
					if(syn->lex->type != LEFT_BRACKET) {
						Syn *r = nu_st.top();
						nu_st.pop();
						Syn *l = nu_st.top();
						nu_st.pop();
						insert(l, r, syn);
						nu_st.push(syn);
					} else {
						++i;
						break;
					}
				}
			} else {
				if(op_st.empty()) {
					Syn *syn = generate_syntax_node(lex_stream + i);
					op_st.push(syn);
					++i;
				} else {
					Syn *syn = op_st.top();
					int lop = syn->lex->type;
					int rop = type;
					printf("%s vs %s\n", lextype[lop],lextype[rop]);
					printf("sup(%s,%s) = %d", lextype[lop],lextype[rop], sup(lop,rop));
					if(lop != LEFT_BRACKET && sup(lop, rop)) {
						printf("%s > %s\n", lextype[lop],lextype[rop]);
						op_st.pop();
						Syn *r = nu_st.top();
						nu_st.pop();
						Syn *l = nu_st.top();
						nu_st.pop();
						insert(l, r, syn);
						nu_st.push(syn);
					} else {
						Syn *syn = generate_syntax_node(lex_stream + i);
						op_st.push(syn);
						printf("top = %s\n", lextype[op_st.top()->lex->type]);
						++i;
					}
				}
			}
		}
	}
}

void print_syn_tree(Syn *syn, int width) {
	if(syn == 0) return;
	int i = 0;
	for(i = 0; i < width; ++i) {
		printf(" ");
	}
	int delta = printf("%s",syn->lex->val);
	printf("\n");
	print_syn_tree(syn->lson, width + delta);
	print_syn_tree(syn->rson, width + delta);
}

int main() {
	char s[MAX_N];
	scanf("%s",s);
	lexical_analyze(s, strlen(s));
	print_lex_list();
	printf("lexical_analyze has finished.\n");
	init_superior_table();
	syntactic_analyze(lex_stream);
	print_syn_tree(nu_st.top(), 0);
}
