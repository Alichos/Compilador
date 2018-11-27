#include <iostream>
#include <vector>
#include <stack>
#include <stdio.h>
#include <string.h>
#include <set>
#include <map>
#include <string>
#include <queue>
#include <stdlib.h> 
#include <sstream>
using namespace std;

class Nodo{
	int index;
	bool es_final;
	vector< pair<Nodo*, char> > aristas;
	public:
		void cambiaEstadoFinal(bool cambia){ es_final = cambia; }

		void defineIndice(int i){ index = i; }

		pair<Nodo*, char> getArista(int i){ return aristas[i]; }

		int getSize(){ return aristas.size(); }

		Nodo* getHijo(int i){ return aristas[i].first; }

		int getEstadoFinal(){ return es_final; }

		char getTransicion(int i){ return aristas[i].second; }

		int getIndice(){ return index; }

		void agregaArista(Nodo* v, char transicion){
			aristas.push_back(make_pair(v, transicion));
		}

		Nodo(){
			index = -1;
			es_final = false;
		}

		Nodo(int indice, bool final){
			index = indice;
			es_final = final;
		}
};

class Automata{
	int numero_nodos;
	int estado_inicial;

	string abecedario;
	string tipo_automata;

	Nodo* nodo_inicial;

	Nodo* buscaNodo(int indice_u){
		stack< Nodo* > pilaRecursiva;
		pilaRecursiva.push(nodo_inicial);

		vector<bool> visitados(numero_nodos, false);
		visitados[nodo_inicial -> getIndice()] = true;
		while(!pilaRecursiva.empty()){
			Nodo* actual = pilaRecursiva.top();
			pilaRecursiva.pop();

			if(actual->getIndice() == indice_u) return actual;

			for(int i = 0; i < actual->getSize(); i++){
				if(!visitados[actual->getHijo(i)->getIndice()]){
					visitados[actual->getHijo(i)->getIndice()] = true;
					pilaRecursiva.push(actual->getHijo(i));
				}
			}
		} return NULL;
	}

	bool Verifica(Nodo* u, char transicion){
		for(int i = 0; i < u -> getSize(); i++){
			if(u -> getTransicion(i) == transicion){
				printf("Recuerda, no puede haber ambiguedad en un AFD\n");
				return false;
			}
		} return true;
	}

	void DefineNumeroEstados(int numero_estados){
		abecedario = "";
		numero_nodos = numero_estados;
	}

	void DefineAbecedario(string abecedario_recibido){
		abecedario += abecedario_recibido;
		if(tipo_automata == "AFN") abecedario += "?";
	}

	void DefineTransiciones(int transiciones){
		printf("Definir automatada...recuerda, el numero de nodos/estados, esta indexado desde 0\n");
		for(int i = 0; i < transiciones; i++){
			char transicion;
			int indice_u, indice_v;
			printf("%d ) Ingresa 2 enteros, u, v, para expresar que u esta conectado con v e ingresa un caracter para definir la transicion\n>", i);
			cin >> indice_u >> indice_v >> transicion;
			if(indice_u >= numero_nodos || indice_v >= numero_nodos){
                printf("Ese indice de nodo no existe en el automata, intentalo de nuevo\n");
				i--; continue;
			}

			Nodo* u = buscaNodo(indice_u);
			Nodo* v = buscaNodo(indice_v);
			if(u == NULL) u = new Nodo(indice_u, false);
			if(v == NULL) v = new Nodo(indice_v, false);

			bool existe = false;
			for(int j = 0; j < abecedario.size(); j++){
				if(abecedario[j] == transicion) existe = true;
			} if(!existe) { printf("El caracter que ingresaste no existe en el abecedario\n"); i--; continue; }

			if(tipo_automata == "AFD"){
				if(!Verifica(u, transicion)){ i--; continue; }
			}

			u -> agregaArista(v, transicion);
		}
	}

	void DefineEstadosFinales(int estados_finales){
		for(int i = 0; i < estados_finales; i++){
			int indice_u;
			printf("%d) Ingresa un entero u, que definira que estado/nodo sera final\n>");
			cin >> indice_u;
			Nodo* u = buscaNodo(indice_u);
			u -> cambiaEstadoFinal(true);
		}
	}

	void DefineEstadoInicial(int estado_inicial_param){
		nodo_inicial = new Nodo(estado_inicial_param, false);
	}

	void DefineTipoAutomata(){
		string query;
		printf("El automata sera AFD o AFN?(AFD, AFN)\n>");
		cin >> tipo_automata;
	}

	public:
		Automata(int numero_estados, string abecedario_recibido, int transiciones, int numero_estados_finales, int est_inicial){
			DefineEstadoInicial(est_inicial);
			DefineNumeroEstados(numero_estados);
			DefineTipoAutomata();
			DefineAbecedario(abecedario_recibido);
			DefineTransiciones(transiciones);
			DefineEstadosFinales(numero_estados_finales);
		}

		Automata(Nodo* grafo, string abecedario_recibido, string tipo_AF){
			abecedario_recibido = abecedario;
			nodo_inicial = grafo;
			tipo_automata = tipo_AF;
		}

		bool VerificaCadena(string cadena){
			bool resultado = false;

			stack< pair<Nodo*, int> > pila;
			pila.push( make_pair(nodo_inicial, 0) );

			while(!pila.empty()){
				Nodo* estado_actual = pila.top().first;
				int indice = pila.top().second;
				pila.pop();

				if(indice == cadena.size()){
					if(estado_actual->getEstadoFinal()){
						resultado = true;
					} else {
						continue;
					}
				}

				for(int i = 0; i < estado_actual->getSize(); i++){
					if(estado_actual->getTransicion(i) == cadena[indice]){
						pila.push(make_pair(estado_actual->getHijo(i), indice + 1));
					}

					if(estado_actual->getTransicion(i) == '?'){
						pila.push(make_pair(estado_actual->getHijo(i), indice));
					}
				}
			} return resultado;
		}
};

typedef string ExpresionRegular;
typedef pair<Nodo*, Nodo*> AF;

//Cambiar esto a un precalculo
int encuentraInicioParentesis(const ExpresionRegular& expresionRegular, int posicion_parentesis){
	int cerrados = 0;
	for(int i = posicion_parentesis; i >= 0; i--){
		if(expresionRegular[i] == '\''){
			if(expresionRegular[i - 1] == ')') cerrados++;                  																		
			if(expresionRegular[i - 1] == '('){ 																									
				cerrados--; if(cerrados == 0) return i;
			}
		}
	}
}

AF ExpresionRegularAAFN(const ExpresionRegular& expresionRegular, int inicio, int final, int& numero_indice){
	vector<int> posicion_ors;
	for(int i = final; i >= inicio; i--){
		if(expresionRegular[i] == '\'' && expresionRegular[i - 1] == ')')                                                                 
			i = encuentraInicioParentesis(expresionRegular, i) - 1;

		if(expresionRegular[i] == '\'' && expresionRegular[i - 1] == '|'){ 
			posicion_ors.push_back(i - 1);             
		}      				  
	}

	Nodo* nodo_inicio = new Nodo(numero_indice++, false);
	Nodo* nodo_final = new Nodo(numero_indice++, false);

	if(posicion_ors.size() > 0){
		int actual_inicio = final;
		posicion_ors.push_back(inicio - 2);
		for(int i = 0; i < posicion_ors.size(); i++){
			//printf("%d == %d\n", actual_inicio, posicion_ors[i] + 1);
			AF actual = ExpresionRegularAAFN(expresionRegular, posicion_ors[i] + 2, actual_inicio, numero_indice); 
			actual.second->cambiaEstadoFinal(false);

			nodo_inicio->agregaArista(actual.first, '?'); actual.second->agregaArista(nodo_final, '?');
			actual_inicio = posicion_ors[i] - 1;
		}
		nodo_final->cambiaEstadoFinal(true);
		return AF(nodo_inicio, nodo_final);
	}

	Nodo* actual = nodo_final; 
	nodo_final->cambiaEstadoFinal(true);
	for(int i = final; i >= inicio; i--){
		if((expresionRegular[i] >= '(' && expresionRegular[i] <= 'z') || expresionRegular[i] == ' ' || expresionRegular[i] == '%'){
			Nodo* nuevo_nodo = new Nodo(numero_indice++, false);
			nuevo_nodo->agregaArista(actual, expresionRegular[i]);
			actual = nuevo_nodo;
		}

		if(expresionRegular[i] == '\'' && (expresionRegular[i - 1] == '+' || expresionRegular[i - 1] == '*')){                                                          
			if(i - 2 < 0) continue;
			Nodo* inicio_cerradura = new Nodo(numero_indice++, false);
			if( (expresionRegular[i - 2] >= '(' && expresionRegular[i - 2] <= 'z') || expresionRegular[i - 2] == ' ' || expresionRegular[i - 2] == '%'){
				Nodo* temporal_inicio = new Nodo(numero_indice++, false);
				Nodo* temporal_final = new Nodo(numero_indice++, false);
 
				temporal_inicio->agregaArista(temporal_final, expresionRegular[i - 2]);
				temporal_final->agregaArista(actual, '?'); 
				
				inicio_cerradura->agregaArista(temporal_inicio,'?');

				temporal_final->agregaArista(temporal_inicio, '?');
			}

			bool esClinton = (expresionRegular[i - 1] == '*')? true : false;

			bool entroRecursion = false;
			if(expresionRegular[i - 2] == '\'' && expresionRegular[i - 3] == ')'){                                                                                                   
				entroRecursion = true;
				int posicion_parentesis_abierto = encuentraInicioParentesis(expresionRegular, i - 2);
				AF parentesis = ExpresionRegularAAFN(expresionRegular, posicion_parentesis_abierto + 1, i - 4, numero_indice);
				
				parentesis.second->agregaArista(actual, '?');
				parentesis.second->cambiaEstadoFinal(false);

				inicio_cerradura->agregaArista(parentesis.first, '?');
				parentesis.second->agregaArista(parentesis.first, '?');

				i = posicion_parentesis_abierto - 1;
			}

			if(expresionRegular[i - 1] == '*' || (entroRecursion && esClinton)){                                                                                   
				inicio_cerradura->agregaArista(actual, '?');
			}

			actual = inicio_cerradura;

			if(expresionRegular[i - 1] != '\'' && ((expresionRegular[i - 2] >= '(' && expresionRegular[i - 2] <= 'z') || expresionRegular[i - 2] == ' ' || expresionRegular[i - 2] == '%')) i = i - 2;
		}

		//El error 2017/05 no esta aqui
		if(expresionRegular[i] == '\'' && expresionRegular[i - 1] == ')'){  																					
			int posicion_parentesis_abierto = encuentraInicioParentesis(expresionRegular, i);
			AF parentesis = ExpresionRegularAAFN(expresionRegular, posicion_parentesis_abierto + 1, i - 2, numero_indice);	
			parentesis.second->cambiaEstadoFinal(false);
			//printf("%d === %d\n", posicion_parentesis_abierto + 1, i - 1);
			i = posicion_parentesis_abierto - 1;

			if(actual != nodo_final){
				//Derp, necesito todos los hijos
				for(int k = 0; k < actual->getSize(); k++){
					parentesis.second -> agregaArista(actual->getHijo(k), actual->getTransicion(k));
				}
			} else{
				nodo_final = parentesis.second;
			} delete actual; actual = parentesis.first;
		}
	}

	//printf("%d\n", inicio);
	//printf("%d -------- \n", nodo_final->getIndice());
	return AF(actual, nodo_final);
}

//Este no es necesario, solamente es para ver el AFN
void ObtenAF(Nodo* automata_finito){
	stack<Nodo*> pila;
	vector<bool> visitados(100000, false);

	pila.push(automata_finito);
	visitados[automata_finito->getIndice()] = true;
	
	while(!pila.empty()){
		Nodo* actual = pila.top();
		pila.pop();

		if(actual->getEstadoFinal()){
			printf("%d es final\n", actual->getIndice());
		} 

		for(int i = 0; i < actual->getSize(); i++){
			printf("%d -> %d con %c\n", actual->getIndice(), actual->getHijo(i)->getIndice(), actual->getTransicion(i));

			if(visitados[actual->getHijo(i)->getIndice()]) continue;

			pila.push(actual->getHijo(i));
			visitados[actual->getHijo(i)->getIndice()] = true;
		}
	} 
}

//Tecnicamente no recibe un solo nodo, deberia recibir un conjunto de nodos.
set<Nodo*> estadoCerraduraEpsilon(set<Nodo*> conjunto){
	stack<Nodo*> pila;
	vector<bool> visitados(100000, false);

	set<Nodo*> cerraduraEpsilon;
	for(auto nodo : conjunto){
		visitados[nodo->getIndice()] = true;
		cerraduraEpsilon.insert(nodo);
		pila.push(nodo);
	}

	//Aqui para abajo no se toca esta bie
	while(!pila.empty()){
		Nodo* actual = pila.top();
		pila.pop();

		cerraduraEpsilon.insert(actual);

		for(int i = 0; i < actual->getSize(); ++i){
			if(actual->getTransicion(i) == '?'){		
				if(visitados[actual->getHijo(i)->getIndice()]) continue;
				visitados[actual->getHijo(i)->getIndice()] = true;
				pila.push(actual->getHijo(i));
			}
		}
	} return cerraduraEpsilon;
}

Nodo* TransformaAFN(Nodo* AFN, const string& abecedario, Nodo* nodo_final){
	int indice = 0;
	
	Nodo* AFD = new Nodo(indice++, false);

	//kernel, nodo
	map<set<Nodo*>, Nodo*> visitados;
	stack< pair<set<Nodo*>, Nodo*> > pila;

	set<Nodo*> conjuntoEAFN; conjuntoEAFN.insert(AFN);
	
	pila.push(make_pair(conjuntoEAFN, AFD));
	visitados[pila.top().first] = pila.top().second;
	
	while(!pila.empty()){
		set<Nodo*> kernelActual = pila.top().first;
		Nodo* nodo_actual = pila.top().second;
		pila.pop();

		set<Nodo*> conjuntoEpsilonActual = estadoCerraduraEpsilon(kernelActual);
		for(auto nodo : conjuntoEpsilonActual){
			if(nodo->getIndice() == nodo_final->getIndice()){
				nodo_actual->cambiaEstadoFinal(true); break;
			}
		}

		for(int k = 0; k < abecedario.size(); ++k){
			bool huboTransicion = false;
			set<Nodo*> nuevoConjunto;
			for(auto nodo: conjuntoEpsilonActual){
				for(int j = 0; j < nodo->getSize(); ++j){
					if(nodo->getTransicion(j) == abecedario[k]){
						nuevoConjunto.insert(nodo->getHijo(j));
						huboTransicion = true;
					}
				}
			}

			if(!huboTransicion) continue;

			Nodo* nodo_creado;
			if(visitados.find(nuevoConjunto) == visitados.end()){
				nodo_creado = new Nodo(indice++, false);
				visitados[nuevoConjunto] = nodo_creado;
				pila.push(make_pair(nuevoConjunto, nodo_creado));
			} else {
				nodo_creado = visitados[nuevoConjunto];
			} nodo_actual->agregaArista(nodo_creado, abecedario[k]);
		}	
	}
	return AFD;
}

void quitarEspacios(int& indice, string cadena){
	for(; indice < cadena.size(); ++indice){
		if(cadena[indice] == ' ') continue;
		else break;
	}
}

pair<Automata, string> GeneraAutomata(ExpresionRegular expresionRegular, string& abecedario, string token){
	int indice = 0;
	AF af = ExpresionRegularAAFN(expresionRegular, 0, expresionRegular.size() - 1, indice);
	Nodo* AFD = TransformaAFN(af.first, abecedario, af.second);
	//Justo aqui destruir el AFN
	return make_pair(Automata(AFD, abecedario, "AFD"), token);
}

string dentroDelContexto(vector< pair<Automata, string> >& automatas, string cadena){
	for(int i = 0; i < automatas.size(); ++i){
		if(automatas[i].first.VerificaCadena(cadena)) return automatas[i].second;
	} return "No hay coincidencias";
}

const string abecedario_expresion = "(' |'a|'b|'c|'d|'e|'f|'g|'h|'i|'j|'k|'l|'m|'n|'o|'p|'q|'r|'s|'t|'u|'v|'w|'x|'y|'z|'A|'B|'C|'D|'E|'F|'G|'H|'I|'J|'K|'L|'M|'N|'O|'P|'Q|'R|'S|'T|'U|'V|'W|'X|'Y|'Z)'";
const string numeros_expresion = "('1|'2|'3|'4|'5|'6|'7|'8|'9)'";

const string expresionRegularCero = "0";
const string expresionRegularId = abecedario_expresion + "+'('0|'1|'2|'3|'4|'5|'6|'7|'8|'9)'*'";
const string expresionRegularN = "('('+|'?|'-)'" + numeros_expresion + "('0|'1|'2|'3|'4|'5|'6|'7|'8|'9)'*')'|'('" + expresionRegularCero + ")'";
const string expresionRegularString = "%('('" + abecedario_expresion + ")'|'('" + expresionRegularN + ")')'+'%";
const string expresionRegularIdAndN = "('('" + expresionRegularId + ")'|'('" + expresionRegularN + ")'|'('" + expresionRegularString + ")')'";
const string expresionRegularC = "('('" + expresionRegularIdAndN + ")',)'*'('" + expresionRegularIdAndN + ")'";
const string expresionRegularAsig = "('('" + expresionRegularId +  ")',('" + expresionRegularIdAndN + ")')'";

bool ERROR;

vector< string > tokens_reales;
vector< pair<Automata, string> > automatas_semanticos;

vector< string > analizadorLexico(){
	string cadena_recibida;

	vector< string > tokens;
	while(getline(cin, cadena_recibida)){
		int indice = 0;
		quitarEspacios(indice, cadena_recibida);

		if(indice == cadena_recibida.size()){ continue; }

		bool espacioParentesis = false;

		//Identificar token
		string nuevo_token = "";
		for(; indice < cadena_recibida.size(); ++indice){
			if(cadena_recibida[indice] == ' ' || cadena_recibida[indice] == '('){
				espacioParentesis = (cadena_recibida[indice] == ' ')? true : false; break;
			} nuevo_token += cadena_recibida[indice];
		}

		if(espacioParentesis){
			//Significa que deberia seguir un nombre de variable;
			quitarEspacios(indice, cadena_recibida);

			int stringEncontrado = 0;
			string cadena_para_analizar = "";
			for(; indice < cadena_recibida.size(); ++indice){
				if(cadena_recibida[indice] == '%') stringEncontrado = (stringEncontrado + 1) % 2;
				if(cadena_recibida[indice] == ' ' && stringEncontrado == 0) break;
				cadena_para_analizar += cadena_recibida[indice];
			} tokens.push_back(nuevo_token); 
			tokens.push_back(cadena_para_analizar);
		
		} else {
			//Tiene que haber una condicions
			tokens.push_back(nuevo_token); 

			int final;
			string analizar_con_automata = "";
			for(final = cadena_recibida.size() - 1; final >= 0; --final){
				if(cadena_recibida[final] == ')') break;
			}

			int stringEncontrado = 0;
			for(; indice <= final; ++indice){
				if(cadena_recibida[indice] == '%') stringEncontrado = (stringEncontrado + 1) % 2;
				if(cadena_recibida[indice] == ' ' && stringEncontrado == 0) continue;
				analizar_con_automata += cadena_recibida[indice];
			} tokens.push_back(analizar_con_automata);
		}
	}

	string abecedario = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ;()+-0123456789,% \20";

	//Automatas semanticos
	automatas_semanticos.push_back(GeneraAutomata( expresionRegularString , abecedario, "string" ));
	automatas_semanticos.push_back(GeneraAutomata( expresionRegularN, abecedario, "int"));
	automatas_semanticos.push_back(GeneraAutomata( expresionRegularId, abecedario, "id"));

	vector< pair<Automata, string> > automatas;
	automatas.push_back(GeneraAutomata( "asig"     , abecedario , "a" ));
	automatas.push_back(GeneraAutomata( "if"       , abecedario , "if" ));
	automatas.push_back(GeneraAutomata( "imprimir", abecedario, "i"));
	automatas.push_back(GeneraAutomata( "sumar", abecedario, "s"));
	automatas.push_back(GeneraAutomata( "while"    , abecedario , "while" ));
	automatas.push_back(GeneraAutomata( expresionRegularN       , abecedario, "n" ));
	automatas.push_back(GeneraAutomata( expresionRegularId + ";" , abecedario, "id" ));
	automatas.push_back(GeneraAutomata( "(" + expresionRegularC + ")" , abecedario, "ciw" ));
	automatas.push_back(GeneraAutomata( "('int|'char|'string|'double)'" , abecedario , "v" ));
	automatas.push_back(GeneraAutomata( "(('mayorque|'menorque)'(('" + expresionRegularC + ")'))", abecedario, "c" )); 

	vector< string > tokens_transformados;
	for(int i = 0; i < tokens.size(); ++i){
		tokens_transformados.push_back( dentroDelContexto(automatas, tokens[i]) );
		
		if(tokens_transformados[i] == "id") tokens_reales.push_back(tokens[i].substr(0, tokens[i].size() - 1));
		else tokens_reales.push_back(tokens[i]);

		if(tokens_transformados[ tokens_transformados.size() - 1] == "No hay coincidencias\n"){ 
			ERROR = true;
		}
	} return tokens_transformados;
}

//Podria hacer un acumulado para optimizar
vector< string > detalleReglaGramatical(string regla_gramatical){
	regla_gramatical += ",";

	vector<string> reglas;
	string nueva_regla = "";
	for(int i = 0; i < regla_gramatical.size(); ++i){
		if(regla_gramatical[i] == ','){ 
			reglas.push_back(nueva_regla);
			nueva_regla = "";
			continue;
		} nueva_regla += regla_gramatical[i];
	} return reglas;
}

struct EstadoGramatica{
	int ind_token;
	int ind_gramatica;
	int ind_regla;

	EstadoGramatica(int t, int g, int r):
	 ind_token(t), ind_gramatica(g), ind_regla(r) {}
};

bool checaGramatica(vector< string > tokens, vector< string > gramatica){
	for(int i = 0; i < tokens.size(); i++){
		cout << tokens[i] << " ";
	} cout << "\n";

	int indice_tokens = 0;
	//Indice de token, En que gramatica voy.
	queue< EstadoGramatica > cola;
	cola.push(EstadoGramatica(indice_tokens, 0, 1));

	int verificacion = 0;
	while(!cola.empty()){
		int indice_token_actual = cola.front().ind_token;
		int indice_gramatica = cola.front().ind_gramatica;
		int indice_regla = cola.front().ind_regla;
		cola.pop();

		vector< string > reglas = detalleReglaGramatical(gramatica[indice_gramatica]);
		if(indice_regla > reglas.size() || indice_token_actual >= tokens.size()) continue;
	

		if(reglas[indice_regla][0] >= 'A' && reglas[indice_regla][0] <= 'Z'){
			for(int i = 0; i < gramatica.size(); ++i){
				if(gramatica[i][0] == reglas[indice_regla][0]){
					cola.push(EstadoGramatica(indice_token_actual, i, 1));
				}
			} continue;
		}
		
		cout << indice_token_actual << " " << tokens[indice_token_actual] << " " << reglas[indice_regla] << "\n";
		if(reglas[indice_regla] == tokens[indice_token_actual]){
			++indice_token_actual; ++indice_regla;
			verificacion = max(verificacion, indice_token_actual);
			cola.push(EstadoGramatica(indice_token_actual, indice_gramatica, indice_regla)); //POR QUE????
		} 
	} if(verificacion < tokens.size()) return false;
	return true;
}


//======================================================================== AUTOMATA LL(0) Componentes ================================================================================
map< string, int > visitados_primeros;
map< string, set< string > > conjuntos_reglas;
void primero(vector< string >& gramatica, string reglas){
	//cout << reglas << " ::  Primero :: \n";
	if(visitados_primeros.find(reglas) != visitados_primeros.end()){ return; }
	visitados_primeros[reglas] = 1;

	//cout << "Prueba primeros:: " << reglas <<"\n";
	vector< string > vector_reglas = detalleReglaGramatical(reglas);
	//if(conjuntos_reglas.find(reglas) == conjuntos_reglas.end()){
	//	set< string > nuevo_conjunto;
	//	conjuntos_reglas[reglas] = nuevo_conjunto;
	//}


	for(int i = 0; i < vector_reglas.size(); ++i){
		//cout << "Prueba primeros for:: " << vector_reglas[i] << "\n";

		if(vector_reglas[i][0] >= 'a' && vector_reglas[i][0] <= 'z'){
			conjuntos_reglas[reglas].insert(vector_reglas[i]);
			return;
		}

		if(vector_reglas[i][0] >= 'A' && vector_reglas[i][0] <= 'Z'){
			//primero(gramatica, vector_reglas[i]);

			//Si se extiende a que las reglas de produccion son strings 
			//mayusculas de mas de un caracter esto debe cambiar
			for(int j = 0; j < gramatica.size(); ++j){
				if(gramatica[j][0] == vector_reglas[i][0]){
					string subcadena_gramatica = gramatica[j].substr(2, gramatica[j].size() - 1);
					//cout << subcadena_gramatica << "--\n";
					primero(gramatica, subcadena_gramatica);

					for(auto k : conjuntos_reglas[subcadena_gramatica]){
						//cout << k << "---\n";
						conjuntos_reglas[vector_reglas[i]].insert(k);
					}
				}
			}

			bool epsilon = false;
			for(auto k : conjuntos_reglas[vector_reglas[i]]){
				if(k == "?") { epsilon = true; continue; }
				conjuntos_reglas[reglas].insert(k);
			} if(!epsilon) return;
		}
	} conjuntos_reglas[reglas].insert("?");
}

map< string, int > visitados_siguientes;
map< string, set< string > > conjuntos_siguientes;
void siguiente(vector< string >& gramatica, string regla){
	//Caso
	//cout << ":: " << regla << "\n";
	if(visitados_siguientes.find(regla) != visitados_siguientes.end()) return;
	
	visitados_siguientes[regla] = 1;

	vector< string > primer_regla = detalleReglaGramatical(gramatica[0]);
	if(regla == primer_regla[0]){
		conjuntos_siguientes[regla].insert("$");
	}

	for(int i = 0; i < gramatica.size(); ++i){
		vector < string > reglas_actuales = detalleReglaGramatical(gramatica[i]);
		if(reglas_actuales[reglas_actuales.size() - 1] == regla){
			siguiente(gramatica,reglas_actuales[0]);
			for(auto k : conjuntos_siguientes[reglas_actuales[0]]){
				conjuntos_siguientes[regla].insert(k);
			}
		}

		string a_buscar = "";
		bool regla_encontrada = false;
		for(int j = 1; j < reglas_actuales.size(); ++j){
			if(regla_encontrada){
				a_buscar += reglas_actuales[j];
				if(j + 1 != reglas_actuales.size()) 
					a_buscar += ",";
			}

			if(reglas_actuales[j] == regla){
				regla_encontrada = true;
				continue;
			}
		}

		if(a_buscar == "") continue;

		bool caso_especial = false;
		primero(gramatica, a_buscar);
		for(auto k : conjuntos_reglas[a_buscar]){
			if(k == "?"){
				caso_especial = true;
				break;
			}
		}

		if(caso_especial){
			siguiente(gramatica, reglas_actuales[0]);
			for(auto k : conjuntos_siguientes[reglas_actuales[0]]){
				conjuntos_siguientes[regla].insert(k);
			}
		} else {
			for(auto k : conjuntos_reglas[a_buscar]){
				conjuntos_siguientes[regla].insert(k);
			}
		}
	}
}

void compruebaAmbiguedad(string regla, string k, int i, map< pair< string, string >, int >& tabla){
	if(tabla.find(make_pair(regla, k)) == tabla.end()){ 
		//cout << "Nuevo -> " << regla << " " << k << " " << i << "\n"; 
		tabla[make_pair(regla, k)] = i;
	} else {
		//cout << "Repetido-> " << regla << " " << k << " " << i  << "\n"; 
		cout << "ERROR - Ambiguedad\n";
	}
}

map< pair< string, string >, int > tabla;
void generaTabla(vector< string > gramatica){
	tabla.clear();
	for(int i = 0; i < gramatica.size(); ++i){
		string regla_i = gramatica[i].substr(2, gramatica[i].size());

		primero(gramatica, regla_i);
		set < string > conjunto_actual = conjuntos_reglas[regla_i];
		
		bool hayEpsilon = false;
		regla_i = gramatica[i][0];
		for(auto k : conjunto_actual){
			if(k == "?"){ 
				siguiente(gramatica, regla_i);
				set< string > conjunto_actual_siguiente = conjuntos_siguientes[regla_i];
				for(auto p : conjunto_actual_siguiente){
					compruebaAmbiguedad(regla_i, p, i, tabla);
				}
			} compruebaAmbiguedad(regla_i, k, i, tabla);
		}
	}
}

//==================================================================== AUTOMATA LR(0) =====================================================================

//==================================================================== Separar Argumentos de Tokens_reales ==================================================================

vector < string > ObtenArgumentos(string argumentos){
	string nuevo_argumento = "";
	vector < string > nuevos_argumentos;
	for(int i = 1; i < argumentos.size() - 1; ++i){
		if(argumentos[i] == ','){
			nuevos_argumentos.push_back(nuevo_argumento);
			nuevo_argumento = "";
			continue;
		} nuevo_argumento += argumentos[i];
	} nuevos_argumentos.push_back(nuevo_argumento);
	return nuevos_argumentos;
}

// ============================================================== Automata LL(0) To use ==================================================================================
//map< string, string > TipoDatoId;
class Dato{ 
	protected:
		int identificador;
		string tipoDato;
	public: 
		Dato(int id): identificador(id) {};
		Dato(){};

		int getIdentificador(){
			return identificador;
		}

		void setTipoDato(string dato){
			tipoDato = dato;
		}

		string getTipoDato(){
			return tipoDato;
		}

		virtual int getInt(){}
		virtual void setInt(int nuevo_valor){}
		virtual string getString(){}
		virtual void setString(string nuevo_valor){}
};

class Entero: public Dato{
	int valor_entero;
	public:
		Entero(int valor): Dato(0), valor_entero(valor) {};
		Entero(): Dato(0) {};
		int getInt(){ return valor_entero; }

		void setInt(int nuevo_valor){ valor_entero = nuevo_valor; }
};

class Cadena: public Dato{
	string valor_string;
	public:
		Cadena(string valor): Dato(1), valor_string(valor){};
		Cadena(): Dato(1) {};

		string getString(){	return valor_string; }

		void setString(string nuevo_valor){ valor_string = nuevo_valor; }
};


map< string, Dato* > ValoresVariables; 

string IntToString (int a){
    ostringstream temp;
    temp << a;
    return temp.str();
}

bool CreaSignificado(vector < string >& argumentos, vector< string >& argumentos_semanticos, int indice){
	if(ValoresVariables.find(argumentos[indice]) == ValoresVariables.end()){
		cout << "Variable no declarada, error semantico\n";
		return false;
	}

	string temporal_argumento = argumentos[indice];
	argumentos_semanticos[indice] = ValoresVariables[argumentos[indice]]->getTipoDato();
	if(argumentos_semanticos[indice] == "string"){
		argumentos[indice] = "%" + ValoresVariables[temporal_argumento]->getString() + "%";
	}
	if(argumentos_semanticos[indice] == "int"){
		argumentos[indice] = "" + IntToString(ValoresVariables[temporal_argumento]->getInt());
	} return true;
}

bool analizadorSintactico(vector< string > gramatica, vector < string > tokens){
	generaTabla(gramatica);

	bool condicionalActivado = false;
	bool regresa = false;

	bool una_vez = false;
	stack < string > pila;
	pila.push("$"); pila.push("S");
	tokens.push_back("$");
	for(int i = 0; i < tokens.size();){
		if(pila.top() == tokens[i] && tokens[i] == "$"){
			cout << "Compilacion exitosa" << "\n";
			return true;
		} 

		if(pila.top() == tokens[i]){	
			//Semantica
			if( pila.top() == "c" ){
				if(tokens[i - 1] == "if" ||
				    tokens[i - 1] == "while"){

					string sentencia_actual = tokens_reales[i].substr(1, tokens_reales[i].size() - 2);
					
					string condicion = ""; int j;
					for(j = 0; j < sentencia_actual.size(); ++j){
						if(sentencia_actual[j] == '(') break;
						condicion += sentencia_actual[j];
					} 

					string parametros = "";
					for(; j < sentencia_actual.size(); ++j){
						parametros += sentencia_actual[j];
					}

					vector < string > argumentos_semanticos;
					vector < string > argumentos = ObtenArgumentos(parametros);

					for(int k = 0; k < argumentos.size(); ++k){
						argumentos_semanticos.push_back(dentroDelContexto(automatas_semanticos, argumentos[k]));
					}

					if(argumentos.size() != 2){
						cout << "Error semantico\n";
						return false;
					}

					vector< int > numeros_a_comparar;
					for(j = 0; j < argumentos_semanticos.size(); ++j){
						if(argumentos_semanticos[j] == "id"){
							if(!CreaSignificado(argumentos, argumentos_semanticos, j)) return false;
						}
						if(argumentos_semanticos[j] == "int"){
							numeros_a_comparar.push_back(atoi(argumentos[j].c_str()));
							continue;
						}
						cout << "Solo se pueden hacen comparaciones entre enteros\n";
						return false;
					} 

					if(condicion == "mayorque"){
						condicionalActivado = (numeros_a_comparar[0] > numeros_a_comparar[1])? false : true;
						if(tokens[i - 1] == "while") regresa = true;
					}
					if(condicion == "menorque"){
						condicionalActivado = (numeros_a_comparar[0] < numeros_a_comparar[1])? false : true;
						if(tokens[i - 1] == "while") regresa = true;
					}

				}
			}

			if(pila.top() == "id"){
				if(tokens[i - 1] == "v"){
					Dato* nuevo_dato;
					if(tokens_reales[i - 1] == "string"){
						Cadena* nueva = new Cadena();
						nuevo_dato = nueva;
					}
					if(tokens_reales[i - 1] == "int"){
						Entero* nuevo = new Entero();
						nuevo_dato = nuevo;
					}

					ValoresVariables[tokens_reales[i]] = nuevo_dato;
					ValoresVariables[tokens_reales[i]]->setTipoDato(tokens_reales[i - 1]);
				}
			}

			//Semantica
			if(pila.top() == "ciw"){
				if(!condicionalActivado){
					vector < string > argumentos_semanticos;
					vector < string > argumentos = ObtenArgumentos(tokens_reales[i]);

					for(int k = 0; k < argumentos.size(); ++k){
						argumentos_semanticos.push_back(dentroDelContexto(automatas_semanticos, argumentos[k]));
					}

					if(tokens[i - 1] == "a" || tokens[i - 1] == "s"){
						if(argumentos.size() != 2){
							cout << "Error semantico\n";
							return false;
						}

						if(argumentos_semanticos[0] == "id"){
							if(ValoresVariables.find(argumentos[0]) == ValoresVariables.end()){
								cout << argumentos[0] << "->";
								cout << "Variable no declarada, error semantico\n";
								return false;
							}

							if(argumentos_semanticos[1] == "id"){
								if(!CreaSignificado(argumentos, argumentos_semanticos, 1)) return false;
							}

							if(ValoresVariables[argumentos[0]]->getTipoDato() == argumentos_semanticos[1]){

								if(tokens[i - 1] == "a"){
									if(argumentos_semanticos[1] == "string"){
										ValoresVariables[argumentos[0]]->setString(argumentos[1].substr(1, argumentos[1].size() - 2));
									}
									if(argumentos_semanticos[1] == "int"){
										int signo = (argumentos[1][0] == '-')? -1 : 1;
										int inicio = (argumentos[1][0] == '-' || argumentos[1][0] == '+')? 1 : 0;
										string nuevo_entero = argumentos[1].substr(inicio , argumentos[1].size());
										ValoresVariables[argumentos[0]]->setInt(atoi(nuevo_entero.c_str()) * signo);
									}
								}

								if(tokens[i - 1] == "s"){
									if(argumentos_semanticos[1] == "string"){
										ValoresVariables[argumentos[0]]->setString(ValoresVariables[argumentos[0]]->getString() + argumentos[1].substr(1, argumentos[1].size() - 2));
									}
									if(argumentos_semanticos[1] == "int"){
										int signo = (argumentos[1][0] == '-')? -1 : 1;
										int inicio = (argumentos[1][0] == '-' || argumentos[1][0] == '+')? 1 : 0;
										string nuevo_entero = argumentos[1].substr(inicio , argumentos[1].size());
										ValoresVariables[argumentos[0]]->setInt(ValoresVariables[argumentos[0]]->getInt() + atoi(nuevo_entero.c_str()) * signo);
									}
								}
							} else {
								cout << "Error - Variables incompatibles\n";
								return false;
							}

						} else {
							cout << "Error, solo se puede asignar a variables\n";
							return false;
						}
					} 

					if(tokens[i - 1] == "i"){
						//Debo imprimir esta uea
						if(argumentos.size() != 1){
							cout << "Error semantico, demasiados argumentos en una funcion \n";
							return false;
						}

						if(argumentos_semanticos[0] == "id"){
							if(!CreaSignificado(argumentos, argumentos_semanticos, 0)) return false;
						}

						if(argumentos_semanticos[0] == "int"){
							cout << argumentos[0] << " :: Valor Imprimido Int \n";
						}
						if(argumentos_semanticos[0] == "string"){
							argumentos[0] = argumentos[0].substr(1, argumentos[0].size() - 2);
							cout << argumentos[0] << " :: Valor Imprimido String \n";
						}
					}

					if(regresa){
						regresa = false;
						//cout << "Regreso el while\n";
						i = i - 4;
						//cout << tokens[i + 1] << " ---  && \n";
					} 
				} else {
					condicionalActivado = false;
					regresa = false;
				}
			} 

			pila.pop(); ++i;
			continue;
		}

		if(pila.top()[0] >= 'A' && pila.top()[0] <= 'Z'){
			vector < string > agregar = detalleReglaGramatical(gramatica[tabla[make_pair(pila.top(), tokens[i])]]);
			pila.pop();
			
			for(int j = agregar.size() - 1; j >= 1; --j){
				pila.push(agregar[j]);
			}		
		} else {
			return false;
		} 
	} return false;
}

// ================================================================================ Automata LR(0) ====================================================================================

int main(){

	vector< string > tokens = analizadorLexico();

	/*for(int i = 0; i < tokens_reales.size(); ++i){
		cout << tokens_reales[i] << " " << tokens[i] << "\n";
	}*/

	if(ERROR){
		cout << "Error lexico.\n";
		return 0;
	}

	vector< string > gramatica;
	gramatica.push_back("S,A");
	gramatica.push_back("A,v,id,A");
	gramatica.push_back("A,I");
	gramatica.push_back("I,a,ciw,I");
	gramatica.push_back("I,i,ciw,I");
	gramatica.push_back("I,s,ciw,I");
	gramatica.push_back("I,P");
	gramatica.push_back("P,if,c,M");
	gramatica.push_back("P,while,c,M"); 
	gramatica.push_back("M,a,ciw,P");
	gramatica.push_back("M,i,ciw,P");
	gramatica.push_back("M,s,ciw,P");
	gramatica.push_back("P,$");

	if(analizadorSintactico(gramatica, tokens)){
		cout << "Busque su archivo como salida.txt\n";
		// 30 noviembre 2017
	} else {
		cout << "ERROR sintactico\n";
	}
	return 0;
}
