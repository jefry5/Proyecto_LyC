#include <iostream>
using namespace std;

enum class EstadoExpresion {Q0,Q1,Q2,Q3,Q4,QF};

EstadoExpresion reconocerDeclaracion(string &cadena);
bool esID(string);
bool esNumero(string);
bool esOperador(string);
string scanner(const string&);

int j = 0; // índice dentro de la cadena fuente

int main(){
	string cadenaFuente;
    cout << "Ingrese la cadenaFuente: ";
    getline(cin, cadenaFuente);
    
	EstadoExpresion declaracion_variable = reconocerDeclaracion(cadenaFuente);
	if(declaracion_variable == EstadoExpresion::QF){
		cout<<"\nSin errores";
	}else{
		cout<<"\nError al reconocer la expresion condicional";
	}

	return 0;
}

string scanner(const string& cadenaFuente) {
    string tok = "";
    while (cadenaFuente[j] == ' ') { // Ignorar espacios en blanco
        j++;
    }
    if (j >= cadenaFuente.length()) {
        return "$"; // fin de cadena
    }
    char c = cadenaFuente[j];
    if (c == '"') { // inicio de una cadena
        tok += c;
        j++;
        c = cadenaFuente[j];
        while (j < cadenaFuente.length() && c != '"') {
            tok += c;
            j++;
            c = cadenaFuente[j];
        }
        if (c == '"') {
            tok += c;
            j++;
        }
    } else if (c >= 'a' && c <= 'z') { // inicio es una letra
        while (j < cadenaFuente.length() && ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'))) {
            tok += c;
            j++;
            c = cadenaFuente[j];
        }
    } else if (isdigit(c) || c == '.') { // inicio es un nro o punto decimal
        bool puntoEncontrado = (c == '.');
        while (j < cadenaFuente.length() && (isdigit(c) || (!puntoEncontrado && c == '.'))) {
            if (c == '.') {
                puntoEncontrado = true;
            }
            tok += c;
            j++;
            c = cadenaFuente[j];
        }
    } else if (c == ',' || c == '(' || c == ')' || c == '=' || c == '*' || c == '/' || c == '-' || c == '+' ||
               c == '<' || c == '>' || c == ';' || c == '[' || c == ']') { // Operador
        tok += c;
        if ((c == '-' && cadenaFuente[j + 1] == '-') ||
            (c == '+' && cadenaFuente[j + 1] == '+') ||
            (c == '<' && cadenaFuente[j + 1] == '=') ||
            (c == '>' && cadenaFuente[j + 1] == '=')) {
            tok += cadenaFuente[j + 1];
            j++;
        }
        j++;
    } else if (c == '$') { // fin de cadena
        tok += c;
    }
    return tok;
}

EstadoExpresion reconocerDeclaracion(string &cadena){
	EstadoExpresion actual = EstadoExpresion::Q0;
	string token;
	bool cadenaRechazada = false;
	while(!cadenaRechazada && token != "$"){
		token = scanner(cadena); //Usar el scanner()
		cout<<"token: "<<token<<endl;
		switch(actual){
			case EstadoExpresion::Q0:
                            if(esID(token) || esNumero(token)){
                                actual = EstadoExpresion::Q1;    
                            }else{
                                cadenaRechazada = true;
                            }
							break;
			case EstadoExpresion::Q1:
							if(token == "<" || token  == ">" || token == "="){  
                                actual = EstadoExpresion::Q2;
                            }else{
                                cadenaRechazada = true;               
                            }
							break;
			case EstadoExpresion::Q2:
                            if(esID(token) || esNumero(token)){
                                actual = EstadoExpresion::Q3;    
                            }else{
                                cadenaRechazada = true;
                            }
							break;				
			case EstadoExpresion::Q3:
							if(token == "$"){
                                actual = EstadoExpresion::QF;
                            }else{
                                cadenaRechazada = true;
                            }
							break;
			case EstadoExpresion::QF:
							break;															
		}
	}
	return actual;
}

bool esID(string token){
	if(!isdigit(token[0]) && !(token[0] == ',' || token[0]== '(' || token[0] == ')' || token[0] == '=' || token[0] == '*' || token[0] == '/' || token[0] == '-' || token[0] == '+' ||
               token[0] == '<' || token[0] == '>' || token[0] == ';' || token[0] == '[' || token[0] == ']')){
		return true;
	}else{
		return false;
	}
}

bool esNumero(string token){
	bool puntoEncontrado = false;
    int longitud = token.length();

    if (longitud == 0) {
        return false; // cadena vacia no es un numero
    }

    for (int i = 0; i < longitud; i++) {
        if (token[i] == '.') {
            if (puntoEncontrado) {
                return false; // mas de un punto no es valido
            }
            puntoEncontrado = true;
        } else if (!isdigit(token[i])) {
            return false; // cualquier caracter que no sea digito o punto hace que no sea un numero
        }
    }

    if (token == "." || token[0] == '.' && token.length() == 1) {
        return false; // un solo punto no es un numero valido
    }

    return true;
}

