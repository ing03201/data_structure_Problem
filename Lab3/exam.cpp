#include <iostream>
#include <deque>

using namespace std;

#define EXIT_SUCCESS 0

#define PLUS 1 // plus minus
#define MULITI 2 // product division
#define BRACKET 3

#define TYPE_PAREN 1
#define TYPE_OPERATOR 2
#define TYPE_NUMBER 3



class Token {
public:
  Token(int t, int v, int p) : type(t), value(v), priority(p) {}
  int type;
  int value;
  int priority;
  friend ostream& operator<<(ostream& out, const Token& obj){
    if (obj.type == TYPE_PAREN)
      out << "token(" << obj.type << ",'" << (char) obj.value << "')";
    else if (obj.type == TYPE_OPERATOR)
      out << "token(" << obj.type << ",'" << (char) obj.value << "')";
    else if (obj.type == TYPE_NUMBER)
      out << "token(" << obj.type << "," << obj.value << ")";
    return out;
  }
};


deque<Token *> tokenDeque;

int parse(char *s) {
  Token *token;
  int type = 0, priority = 0;
  
  while (*s) {
    int value = 0;
    switch(*s)
    {
        case '(': 
            type = TYPE_PAREN; value= '('; s++; priority = BRACKET;
            break;
        case ')': 
            type = TYPE_PAREN; value= ')'; s++;  priority = BRACKET;
            break;
        case '+': 
            type = TYPE_OPERATOR; value= '+'; s++; priority = PLUS; 
            break;
        case '-': 
            type = TYPE_OPERATOR; value= '-'; s++; priority = PLUS; 
            break;
        case '*':
            type = TYPE_OPERATOR; value= '*'; s++; priority = MULITI; 
            break;
        case '/': 
            type = TYPE_OPERATOR; value= '/'; s++; priority = MULITI;
            break;
        default : 
            if(isdigit(*s)){ 
                type = TYPE_NUMBER;
                while(*s && isdigit(*s)) {
                    value = value * 10 + *s++ - '0';
                }
            }
            break;
        
    }

    if (type == 0) {
      cout << "wrong expression!" << endl;
      return -1;
    }
      
    token = new Token(type, value, priority);
    tokenDeque.push_back(token);
  }
  
  return 0;
}


/* Home Work*/
double calculate(deque<int> *n, deque<Token *> *o){
	deque<Token *>::iterator i = o->begin();
	int temp1, temp2;
    Token * op;
	double total = 0;
	while(!o->empty()){
		temp1 = n->front();
		n->pop_front();
		temp2 = n->front();
		n->pop_front();
        op = *i;
		switch(op->value){
			case '(': break;
			case '+': total = (double) temp1 + (double) temp2; break;
			case '-': total = (double) temp2 - (double) temp1; break;
			case '*': total = (double) temp1 * (double) temp2; break;
			case '/': total = (double) temp1 / (double) temp2; break;
			default: break;
		}
		if(total != 0){
			n->push_front(total);
		}
		o->pop_front();
		i++;
	}
	if(o->empty() || n->size() == 1)
		total = n->front();
	return total;
}

double tokenstack(void){
	deque<int> number;
	deque<Token *> oper;
    Token *op;
    int t;
    int v;
    int p;

	double result;
	for(unsigned int i = 0; i < tokenDeque.size(); i++){
		t = tokenDeque[i]->type;
		v = tokenDeque[i]->value;
        p = tokenDeque[i]->priority;
		switch(t){
			case TYPE_PAREN:
				if(v == ')')
					result = calculate(&number, &oper);
					number.push_front(result);
				if(v == '(')
                op = new Token(t, v, p);
				oper.push_front(op);
				break;
			case TYPE_OPERATOR:
                op = new Token(t, v, p);
				oper.push_front(op);
				if(v == '*' || v == '/'){
					i++;
					if(tokenDeque[i]->type == TYPE_NUMBER){
						v = tokenDeque[i]->value;
						number.push_front(v);
						result = calculate(&number, &oper);
						number.push_front(result);
					}
						
				}
				break;
			case TYPE_NUMBER:
				number.push_front(v);
				break;
			default : break;
		}
		
	}
	result = calculate(&number, &oper);
	return result;
}

void print_tokens(void) {
  Token *p;
  int result = tokenstack();
  for (p = tokenDeque.front(); !tokenDeque.empty(); p = tokenDeque.front()) {
    cout << *p << endl;
    tokenDeque.pop_front();
  }
  cout << "calculateulate result = " << result << endl;
}

int main(int argc, char *argv[])
{
  istream& input_stream = cin;
  string input_expr;
	
  cout << "Type any arithmetic express with no spaces such as (l00+200)*300-400/100" << endl;
  while (true) {	
    cout << "> ";
    if (!getline(input_stream, input_expr))
      break;

    if (parse((char *)input_expr.c_str()) < 0)
      continue;

    print_tokens();
  }
  return EXIT_SUCCESS;
}
