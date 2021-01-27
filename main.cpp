#include "../include/main.hpp"

using namespace std;

//VARS GLOBAIS
pthread_mutex_t lock_forno;
pthread_mutex_t lock_p[NTHREADS];
pthread_cond_t  cond_p[NTHREADS];
pthread_mutex_t lock_raj_liberou;
pthread_mutex_t lock_cout;
pthread_mutex_t lock_primeiro_do_casal[3];
pthread_mutex_t lock_casal_junto[3];
bool raj_liberou = false;
int primeiro_do_casal[3];
bool casal_junto[3];


//CLASSES
class Person {
  private:
    int _id;
    string _name;
    bool _quer_usar;

  public:
    Person(int userId, string userName){
      _name = userName;
      _id = userId;
      _quer_usar = false;
    }

    Person(){
      _quer_usar = false;
    }

    void setName(string username){
      _name = username;
    }

    void setId(int userId){
    	_id = userId;
      switch(userId){
        case SHELDON:
          setName("Sheldon");
          break;
        case HOWARD:
          setName("Howard");
          break;
        case LEONARD:
          setName("Leonard");
          break;
        case RAJ:
          setName("Raj");
          break;
        case KRIPKE:
          setName("Kripke");
          break;
        case AMY:
          setName("Amy");
          break;
        case BERNADETTE:
          setName("Bernadette");
          break;
        case PENNY:
          setName("Penny");
          break;
        case STUART:
          setName("Stuart");
          break;
      }
    }

    void setQuer_usar(bool quer_usar){
      _quer_usar = quer_usar;
    }

    string getName(){
    	return _name;
    }

    int getId(){
    	return _id;
    }

    bool getQuer_usar(){
      return _quer_usar;
    }

    void cook_something(){
      sleep(1);
    }

    void eat(){
      srand48(time(NULL));
      int number = 1 + (drand48 () * 2);
      sleep(number);
    }

    void work(){
      srand48(time(NULL));
      int number = 3 + (drand48 () * 3);
      pthread_mutex_lock(&lock_cout);
      cout << _name << " voltou para o trabalho" << "\n";
      pthread_mutex_unlock(&lock_cout);
      sleep(number);
    }
};

//Variáveis globais
Person p[10];

//FUNCOES GLOBAIS

int prioridades(int idPessoa){
  int prior = -1;
  switch(idPessoa){
    case SHELDON:
      prior=LEONARD;
      break;
    case HOWARD:
      prior=SHELDON;
      break;
    case BERNADETTE:
      prior = SHELDON;
      break;
    case LEONARD:
      prior=HOWARD;
      break;
    case STUART:
      prior=STUART;//como o stuart tem mais de um prior, a gente coloca ele mesmo como prior e trata nas funções
      break;
    case KRIPKE:
      prior=KRIPKE;//como o Kripke tem mais de um prior, a gente coloca ele mesmo como prior e trata nas funções
      break;
    default:
      perror("Prioridade não encontrada!");
  }
  return prior;
}

int dependentes(int idPessoa){
  int prior = -1;
  switch(idPessoa){
    case LEONARD:
      prior=SHELDON;
      break;
    case SHELDON:
      prior=HOWARD;
      break;
    case BERNADETTE:
      prior = LEONARD;
      break;
    case HOWARD:
      prior=LEONARD;
      break;
    default:
      perror("Dependentes não encontrada!");
  }
  return prior;
}

int casais(int idPessoa){
  int casal = -1;
  switch(idPessoa){
    case SHELDON:
      casal=AMY;
      break;
    case HOWARD:
      casal=BERNADETTE;
      break;
    case LEONARD:
      casal=PENNY;
      break;
    case BERNADETTE:
      casal=HOWARD;
      break;
    default:
      perror("Casal não encontrado!");
  }
  return casal;
}

int id_casal(int idPessoa){
  int id_casal = -1;
  switch(idPessoa){
    case SHELDON:
      id_casal=0;
      break;
    case AMY:
      id_casal=0;
      break;
    case HOWARD:
      id_casal=1;
      break;
    case BERNADETTE:
      id_casal=1;
      break;
    case LEONARD:
      id_casal=2;
      break;
    case PENNY:
      id_casal=2;
      break;
    default:
      perror("id_Casal não encontrado!");
  }

  return id_casal;
}

void trata_prioridades(Person pessoa){

  int idPessoa = pessoa.getId();
  int prior = prioridades(idPessoa);
  //cout << "--------------------------------------" << pessoa.getName() << ": Seu prior é: " << p[prior].getName() << "\n";
  if((prior!= -1) and (prior != STUART) and (prior != KRIPKE)){
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   
    /*
    SIGNAL GARANTE A ORDEM DE EXECUÇÃO, USE SIGNAL E DÊ OS SINAIS ANTES DOS ULTIMOS UNLOCKS DO WAIT
    MANTENHA A ORDEM DOS LOCKS, PARA GARANTIR QUE NAO TENHA DEADLOCK POR CAUSA DELES
    DE UM SIGNAL ANTES DE DESTRANCAR AS VARIAVEIS NO FIM DO LOOP ETERNO
    - Toda vez que eu executo esta função, eu quero saber se eu posso usar o forno neste exato instante da fila, o que requer verificar todos menos RAJ, STUART e KRIPKE.
    - Eu deveria lockar todo mundo que eu preciso de uma vez, pois assim, eu consigo verificar a fila neste exato instante.
    - Após o lock, eu verifico tudo e espero quem eu precisar(para esperar quem eu preciso, será necessário dar unlock em todos os outros, o que muda o instante verificado).
    - A idéia é que, se todo mundo der unlock nos outros antes de esperar, eventualmente a pessoa que deve usar o forno vai pegar todos os locks,
      verificar que ele não precisa esperar ninguém e vai, enfim usar o forno(que é onde ele vai enviar o sinal e tirar alguém do wait).
    - Pela idéia acima, a gente sabe que as outras threads que pegarem os locks, ou vão só liberar geral(caso do Raj) ou vão esperar alguém.
    - Ao fim das possíveis esperas, verificar se você pode realmente usar o forno(pq alguns unlocks podem ter sido feito, mudando o instante verificado), se não puder
      repita o processo.(A idéia aqui é que você só para de esperar se, no instante em que você deu lock em todas as pessoas, é a sua vez)
    */
    
    
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if(idPessoa != SHELDON)
      pthread_mutex_lock(&lock_p[SHELDON]);
    if(idPessoa != HOWARD)
      pthread_mutex_lock(&lock_p[HOWARD]);
    if(idPessoa != LEONARD)
      pthread_mutex_lock(&lock_p[LEONARD]);
    if(idPessoa != BERNADETTE)
      pthread_mutex_lock(&lock_p[BERNADETTE]);
    if(idPessoa != PENNY)
      pthread_mutex_lock(&lock_p[PENNY]);
    if(idPessoa != AMY)
      pthread_mutex_lock(&lock_p[AMY]);
    while(1){
      int casal = casais(idPessoa);
      int pessoa_dependente = dependentes(idPessoa);
      int casal_pessoa_dependente = casal(pessoa_dependente);
      int casal_prior = casal(prior);
      int id_casal_pessoa = id_casal(idPessoa);

      if(!p[casal].getQuer_usar()){//caso eu não tenha casal na fila
        if(p[pessoa_dependente].getQuer_usar() and p[casal_pessoa_dependente].getQuer_usar()){//se o casal que depende de mim estiver na fila, esperar eles
          while(p[pessoa_dependente].getQuer_usar()){//espero a pessoa que depende de mim
            //se o raj liberou, ignore o while
            pthread_mutex_lock(&lock_raj_liberou);
            if(raj_liberou){
              pthread_mutex_lock(&lock_cout);
              cout << "Raj detectou um deadlock, liberando " << pessoa.getName() << "\n";
              pthread_mutex_unlock(&lock_cout);
              raj_liberou = false;
              pthread_mutex_unlock(&lock_raj_liberou);
              break;
            }
            else
              pthread_mutex_unlock(&lock_raj_liberou);

            if((idPessoa != SHELDON) and (pessoa_dependente != SHELDON))
              pthread_mutex_unlock(&lock_p[SHELDON]);
            if((idPessoa != HOWARD) and (pessoa_dependente != HOWARD))
              pthread_mutex_unlock(&lock_p[HOWARD]);
            if((idPessoa != LEONARD) and (pessoa_dependente != LEONARD))
              pthread_mutex_unlock(&lock_p[LEONARD]);
            if((idPessoa != BERNADETTE) and (pessoa_dependente != BERNADETTE))
              pthread_mutex_unlock(&lock_p[BERNADETTE]);
            if((idPessoa != PENNY) and (pessoa_dependente != PENNY))
              pthread_mutex_unlock(&lock_p[PENNY]);
            if((idPessoa != AMY) and (pessoa_dependente != AMY))
              pthread_mutex_unlock(&lock_p[AMY]);

            pthread_cond_wait(&cond_p[pessoa_dependente], &lock_p[pessoa_dependente]);
            pthread_mutex_unlock(&lock_p[pessoa_dependente]);

            if(idPessoa != SHELDON)
              pthread_mutex_lock(&lock_p[SHELDON]);
            if(idPessoa != HOWARD)
              pthread_mutex_lock(&lock_p[HOWARD]);
            if(idPessoa != LEONARD)
              pthread_mutex_lock(&lock_p[LEONARD]);
            if(idPessoa != BERNADETTE)
              pthread_mutex_lock(&lock_p[BERNADETTE]);
            if(idPessoa != PENNY)
              pthread_mutex_lock(&lock_p[PENNY]);
            if(idPessoa != AMY)
              pthread_mutex_lock(&lock_p[AMY]);
          }

          while(p[casal_pessoa_dependente].getQuer_usar()){//espero o casal da pessoa que depende de mim
            //se o raj liberou, ignore o while
            pthread_mutex_lock(&lock_raj_liberou);
            if(raj_liberou){
              pthread_mutex_lock(&lock_cout);
              cout << "Raj detectou um deadlock, liberando " << pessoa.getName() << "\n";
              pthread_mutex_unlock(&lock_cout);
              raj_liberou = false;
              pthread_mutex_unlock(&lock_raj_liberou);
              break;
            }
            else
              pthread_mutex_unlock(&lock_raj_liberou);

            if((idPessoa != SHELDON) and (casal_pessoa_dependente != SHELDON))
              pthread_mutex_unlock(&lock_p[SHELDON]);
            if((idPessoa != HOWARD) and (casal_pessoa_dependente != HOWARD))
              pthread_mutex_unlock(&lock_p[HOWARD]);
            if((idPessoa != LEONARD) and (casal_pessoa_dependente != LEONARD))
              pthread_mutex_unlock(&lock_p[LEONARD]);
            if((idPessoa != BERNADETTE) and (casal_pessoa_dependente != BERNADETTE))
              pthread_mutex_unlock(&lock_p[BERNADETTE]);
            if((idPessoa != PENNY) and (casal_pessoa_dependente != PENNY))
              pthread_mutex_unlock(&lock_p[PENNY]);
            if((idPessoa != AMY) and (casal_pessoa_dependente != AMY))
              pthread_mutex_unlock(&lock_p[AMY]);

            pthread_cond_wait(&cond_p[casal_pessoa_dependente], &lock_p[casal_pessoa_dependente]);
            pthread_mutex_unlock(&lock_p[casal_pessoa_dependente]);

            if(idPessoa != SHELDON)
              pthread_mutex_lock(&lock_p[SHELDON]);
            if(idPessoa != HOWARD)
              pthread_mutex_lock(&lock_p[HOWARD]);
            if(idPessoa != LEONARD)
              pthread_mutex_lock(&lock_p[LEONARD]);
            if(idPessoa != BERNADETTE)
              pthread_mutex_lock(&lock_p[BERNADETTE]);
            if(idPessoa != PENNY)
              pthread_mutex_lock(&lock_p[PENNY]);
            if(idPessoa != AMY)
              pthread_mutex_lock(&lock_p[AMY]);
          }
        }

        while(p[prior].getQuer_usar()){//espero a pessoa que tem prioridade sobre mim
          //se o raj liberou, ignore o while
          pthread_mutex_lock(&lock_raj_liberou);
          if(raj_liberou){
            pthread_mutex_lock(&lock_cout);
            cout << "Raj detectou um deadlock, liberando " << pessoa.getName() << "\n";
            pthread_mutex_unlock(&lock_cout);
            raj_liberou = false;
            pthread_mutex_unlock(&lock_raj_liberou);
            break;
          }
          else
            pthread_mutex_unlock(&lock_raj_liberou);

          if((idPessoa != SHELDON) and (prior != SHELDON))
            pthread_mutex_unlock(&lock_p[SHELDON]);
          if((idPessoa != HOWARD) and (prior != HOWARD))
            pthread_mutex_unlock(&lock_p[HOWARD]);
          if((idPessoa != LEONARD) and (prior != LEONARD))
            pthread_mutex_unlock(&lock_p[LEONARD]);
          if((idPessoa != BERNADETTE) and (prior != BERNADETTE))
            pthread_mutex_unlock(&lock_p[BERNADETTE]);
          if((idPessoa != PENNY) and (prior != PENNY))
            pthread_mutex_unlock(&lock_p[PENNY]);
          if((idPessoa != AMY) and (prior != AMY))
            pthread_mutex_unlock(&lock_p[AMY]);

          pthread_cond_wait(&cond_p[prior], &lock_p[prior]);
          pthread_mutex_unlock(&lock_p[prior]);

          if(idPessoa != SHELDON)
            pthread_mutex_lock(&lock_p[SHELDON]);
          if(idPessoa != HOWARD)
            pthread_mutex_lock(&lock_p[HOWARD]);
          if(idPessoa != LEONARD)
            pthread_mutex_lock(&lock_p[LEONARD]);
          if(idPessoa != BERNADETTE)
            pthread_mutex_lock(&lock_p[BERNADETTE]);
          if(idPessoa != PENNY)
            pthread_mutex_lock(&lock_p[PENNY]);
          if(idPessoa != AMY)
            pthread_mutex_lock(&lock_p[AMY]);
        }

        while(p[casal_prior].getQuer_usar()){//espero o casal da pessoa que tem prioridade sobre mim
          //se o raj liberou, ignore o while
          pthread_mutex_lock(&lock_raj_liberou);
          if(raj_liberou){
            pthread_mutex_lock(&lock_cout);
            cout << "Raj detectou um deadlock, liberando " << pessoa.getName() << "\n";
            pthread_mutex_unlock(&lock_cout);
            raj_liberou = false;
            pthread_mutex_unlock(&lock_raj_liberou);
            break;
          }
          else
            pthread_mutex_unlock(&lock_raj_liberou);

          if((idPessoa != SHELDON) and (casal_prior != SHELDON))
            pthread_mutex_unlock(&lock_p[SHELDON]);
          if((idPessoa != HOWARD) and (casal_prior != HOWARD))
            pthread_mutex_unlock(&lock_p[HOWARD]);
          if((idPessoa != LEONARD) and (casal_prior != LEONARD))
            pthread_mutex_unlock(&lock_p[LEONARD]);
          if((idPessoa != BERNADETTE) and (casal_prior != BERNADETTE))
            pthread_mutex_unlock(&lock_p[BERNADETTE]);
          if((idPessoa != PENNY) and (casal_prior != PENNY))
            pthread_mutex_unlock(&lock_p[PENNY]);
          if((idPessoa != AMY) and (casal_prior != AMY))
            pthread_mutex_unlock(&lock_p[AMY]);

          pthread_cond_wait(&cond_p[casal_prior], &lock_p[casal_prior]);
          pthread_mutex_unlock(&lock_p[casal_prior]);

          if(idPessoa != SHELDON)
            pthread_mutex_lock(&lock_p[SHELDON]);
          if(idPessoa != HOWARD)
            pthread_mutex_lock(&lock_p[HOWARD]);
          if(idPessoa != LEONARD)
            pthread_mutex_lock(&lock_p[LEONARD]);
          if(idPessoa != BERNADETTE)
            pthread_mutex_lock(&lock_p[BERNADETTE]);
          if(idPessoa != PENNY)
            pthread_mutex_lock(&lock_p[PENNY]);
          if(idPessoa != AMY)
            pthread_mutex_lock(&lock_p[AMY]);
        }
      }
      else{//se eu tiver casal na fila
        //verifico se o meu casal vai primeiro, caso verdadeiro, espero
        pthread_mutex_lock(&lock_primeiro_do_casal[id_casal_pessoa]);
        if(primeiro_do_casal[id_casal_pessoa] == casal){
          pthread_mutex_unlock(&lock_primeiro_do_casal[id_casal_pessoa]);
          while(p[casal].getQuer_usar()){//espero meu casal
            //se o raj liberou, ignore o while
            pthread_mutex_lock(&lock_raj_liberou);
            if(raj_liberou){
              pthread_mutex_lock(&lock_cout);
              cout << "Raj detectou um deadlock, liberando " << pessoa.getName() << "\n";
              pthread_mutex_unlock(&lock_cout);
              raj_liberou = false;
              pthread_mutex_unlock(&lock_raj_liberou);
              break;
            }
            else
              pthread_mutex_unlock(&lock_raj_liberou);

            if((idPessoa != SHELDON) and (casal != SHELDON))
              pthread_mutex_unlock(&lock_p[SHELDON]);
            if((idPessoa != HOWARD) and (casal != HOWARD))
              pthread_mutex_unlock(&lock_p[HOWARD]);
            if((idPessoa != LEONARD) and (casal != LEONARD))
              pthread_mutex_unlock(&lock_p[LEONARD]);
            if((idPessoa != BERNADETTE) and (casal != BERNADETTE))
              pthread_mutex_unlock(&lock_p[BERNADETTE]);
            if((idPessoa != PENNY) and (casal != PENNY))
              pthread_mutex_unlock(&lock_p[PENNY]);
            if((idPessoa != AMY) and (casal != AMY))
              pthread_mutex_unlock(&lock_p[AMY]);

            pthread_cond_wait(&cond_p[casal], &lock_p[casal]);
            pthread_mutex_unlock(&lock_p[casal]);

            if(idPessoa != SHELDON)
              pthread_mutex_lock(&lock_p[SHELDON]);
            if(idPessoa != HOWARD)
              pthread_mutex_lock(&lock_p[HOWARD]);
            if(idPessoa != LEONARD)
              pthread_mutex_lock(&lock_p[LEONARD]);
            if(idPessoa != BERNADETTE)
              pthread_mutex_lock(&lock_p[BERNADETTE]);
            if(idPessoa != PENNY)
              pthread_mutex_lock(&lock_p[PENNY]);
            if(idPessoa != AMY)
              pthread_mutex_lock(&lock_p[AMY]);
          }
        }

        if(p[prior].getQuer_usar() and p[casal_prior].getQuer_usar()){//se a pessoa com prioridade sobre mim esta com o seu casal, espero os dois
          while(p[prior].getQuer_usar()){//espero a pessoa com prioridade sobre mim
            //se o raj liberou, ignore o while
            pthread_mutex_lock(&lock_raj_liberou);
            if(raj_liberou){
              pthread_mutex_lock(&lock_cout);
              cout << "Raj detectou um deadlock, liberando " << pessoa.getName() << "\n";
              pthread_mutex_unlock(&lock_cout);
              raj_liberou = false;
              pthread_mutex_unlock(&lock_raj_liberou);
              break;
            }
            else
              pthread_mutex_unlock(&lock_raj_liberou);

            if((idPessoa != SHELDON) and (prior != SHELDON))
              pthread_mutex_unlock(&lock_p[SHELDON]);
            if((idPessoa != HOWARD) and (prior != HOWARD))
              pthread_mutex_unlock(&lock_p[HOWARD]);
            if((idPessoa != LEONARD) and (prior != LEONARD))
              pthread_mutex_unlock(&lock_p[LEONARD]);
            if((idPessoa != BERNADETTE) and (prior != BERNADETTE))
              pthread_mutex_unlock(&lock_p[BERNADETTE]);
            if((idPessoa != PENNY) and (prior != PENNY))
              pthread_mutex_unlock(&lock_p[PENNY]);
            if((idPessoa != AMY) and (prior != AMY))
              pthread_mutex_unlock(&lock_p[AMY]);

            pthread_cond_wait(&cond_p[prior], &lock_p[prior]);
            pthread_mutex_unlock(&lock_p[prior]);

            if(idPessoa != SHELDON)
              pthread_mutex_lock(&lock_p[SHELDON]);
            if(idPessoa != HOWARD)
              pthread_mutex_lock(&lock_p[HOWARD]);
            if(idPessoa != LEONARD)
              pthread_mutex_lock(&lock_p[LEONARD]);
            if(idPessoa != BERNADETTE)
              pthread_mutex_lock(&lock_p[BERNADETTE]);
            if(idPessoa != PENNY)
              pthread_mutex_lock(&lock_p[PENNY]);
            if(idPessoa != AMY)
              pthread_mutex_lock(&lock_p[AMY]);
          }

          while(p[casal_prior].getQuer_usar()){//espero o casal da pessoa com prioridade sobre mim
            //se o raj liberou, ignore o while
            pthread_mutex_lock(&lock_raj_liberou);
            if(raj_liberou){
              pthread_mutex_lock(&lock_cout);
              cout << "Raj detectou um deadlock, liberando " << pessoa.getName() << "\n";
              pthread_mutex_unlock(&lock_cout);
              raj_liberou = false;
              pthread_mutex_unlock(&lock_raj_liberou);
              break;
            }
            else
              pthread_mutex_unlock(&lock_raj_liberou);

            if((idPessoa != SHELDON) and (casal_prior != SHELDON))
              pthread_mutex_unlock(&lock_p[SHELDON]);
            if((idPessoa != HOWARD) and (casal_prior != HOWARD))
              pthread_mutex_unlock(&lock_p[HOWARD]);
            if((idPessoa != LEONARD) and (casal_prior != LEONARD))
              pthread_mutex_unlock(&lock_p[LEONARD]);
            if((idPessoa != BERNADETTE) and (casal_prior != BERNADETTE))
              pthread_mutex_unlock(&lock_p[BERNADETTE]);
            if((idPessoa != PENNY) and (casal_prior != PENNY))
              pthread_mutex_unlock(&lock_p[PENNY]);
            if((idPessoa != AMY) and (casal_prior != AMY))
              pthread_mutex_unlock(&lock_p[AMY]);

            pthread_cond_wait(&cond_p[casal_prior], &lock_p[casal_prior]);
            pthread_mutex_unlock(&lock_p[casal_prior]);

            if(idPessoa != SHELDON)
              pthread_mutex_lock(&lock_p[SHELDON]);
            if(idPessoa != HOWARD)
              pthread_mutex_lock(&lock_p[HOWARD]);
            if(idPessoa != LEONARD)
              pthread_mutex_lock(&lock_p[LEONARD]);
            if(idPessoa != BERNADETTE)
              pthread_mutex_lock(&lock_p[BERNADETTE]);
            if(idPessoa != PENNY)
              pthread_mutex_lock(&lock_p[PENNY]);
            if(idPessoa != AMY)
              pthread_mutex_lock(&lock_p[AMY]);
          }
        }
      }

      //verifico se eu posso usar o forno agora
      pthread_mutex_lock(&lock_primeiro_do_casal[id_casal_pessoa]);
      if(
        raj_liberou
        or (!p[casal].getQuer_usar() and !(p[pessoa_dependente].getQuer_usar() and p[casal_pessoa_dependente].getQuer_usar()) and !p[prior].getQuer_usar() and !p[casal_prior].getQuer_usar())
        or (p[casal].getQuer_usar() and (primeiro_do_casal[id_casal_pessoa] == idPessoa) and !(p[prior].getQuer_usar() and p[casal_prior].getQuer_usar()))
      ){
        pthread_mutex_unlock(&lock_primeiro_do_casal[id_casal_pessoa]);

        //tranca o forno
        pthread_mutex_lock(&lock_forno); 

        if(idPessoa != SHELDON){
          pthread_cond_signal(&cond_p[SHELDON]);
          pthread_mutex_unlock(&lock_p[SHELDON]);
        }
        if(idPessoa != HOWARD){
          pthread_cond_signal(&cond_p[HOWARD]);
          pthread_mutex_unlock(&lock_p[HOWARD]);
        }
        if(idPessoa != LEONARD){
          pthread_cond_signal(&cond_p[LEONARD]);
          pthread_mutex_unlock(&lock_p[LEONARD]);
        }
        if(idPessoa != BERNADETTE){
          pthread_cond_signal(&cond_p[BERNADETTE]);
          pthread_mutex_unlock(&lock_p[BERNADETTE]);
        }
        if(idPessoa != PENNY){
          pthread_cond_signal(&cond_p[PENNY]);
          pthread_mutex_unlock(&lock_p[PENNY]);
        }
        if(idPessoa != AMY){
          pthread_cond_signal(&cond_p[AMY]);
          pthread_mutex_unlock(&lock_p[AMY]);
        }
        break;
      }
    }
  }
  else
  if((prior == STUART) or (prior == KRIPKE)){
    //tranca o forno
    pthread_mutex_lock(&lock_forno);

    while(1){
      pthread_mutex_lock(&lock_p[SHELDON]);
      pthread_mutex_lock(&lock_p[HOWARD]);
      pthread_mutex_lock(&lock_p[LEONARD]);
      pthread_mutex_lock(&lock_p[BERNADETTE]);
      pthread_mutex_lock(&lock_p[PENNY]);
      pthread_mutex_lock(&lock_p[AMY]);
      if(prior == KRIPKE){
        pthread_mutex_lock(&lock_p[STUART]);
        while(p[STUART].getQuer_usar()){
          pthread_mutex_unlock(&lock_p[SHELDON]);
          pthread_mutex_unlock(&lock_p[HOWARD]);
          pthread_mutex_unlock(&lock_p[LEONARD]);
          pthread_mutex_unlock(&lock_p[BERNADETTE]);
          pthread_mutex_unlock(&lock_p[PENNY]);
          pthread_mutex_unlock(&lock_p[AMY]);

          pthread_cond_wait(&cond_p[STUART], &lock_p[STUART]);

          pthread_mutex_lock(&lock_p[SHELDON]);
          pthread_mutex_lock(&lock_p[HOWARD]);
          pthread_mutex_lock(&lock_p[LEONARD]);
          pthread_mutex_lock(&lock_p[BERNADETTE]);
          pthread_mutex_lock(&lock_p[PENNY]);
          pthread_mutex_lock(&lock_p[AMY]);
        }
      }

      while(p[SHELDON].getQuer_usar()){
        pthread_mutex_unlock(&lock_p[HOWARD]);
        pthread_mutex_unlock(&lock_p[LEONARD]);
        pthread_mutex_unlock(&lock_p[BERNADETTE]);
        pthread_mutex_unlock(&lock_p[PENNY]);
        pthread_mutex_unlock(&lock_p[AMY]);

        pthread_cond_wait(&cond_p[SHELDON], &lock_p[SHELDON]);

        pthread_mutex_lock(&lock_p[HOWARD]);
        pthread_mutex_lock(&lock_p[LEONARD]);
        pthread_mutex_lock(&lock_p[BERNADETTE]);
        pthread_mutex_lock(&lock_p[PENNY]);
        pthread_mutex_lock(&lock_p[AMY]);
      }

      while(p[HOWARD].getQuer_usar()){
        pthread_mutex_unlock(&lock_p[SHELDON]);
        pthread_mutex_unlock(&lock_p[LEONARD]);
        pthread_mutex_unlock(&lock_p[BERNADETTE]);
        pthread_mutex_unlock(&lock_p[PENNY]);
        pthread_mutex_unlock(&lock_p[AMY]);

        pthread_cond_wait(&cond_p[HOWARD], &lock_p[HOWARD]);
        pthread_mutex_unlock(&lock_p[HOWARD]);

        pthread_mutex_lock(&lock_p[SHELDON]);
        pthread_mutex_lock(&lock_p[HOWARD]);
        pthread_mutex_lock(&lock_p[LEONARD]);
        pthread_mutex_lock(&lock_p[BERNADETTE]);
        pthread_mutex_lock(&lock_p[PENNY]);
        pthread_mutex_lock(&lock_p[AMY]);
      }

      while(p[LEONARD].getQuer_usar()){
        pthread_mutex_unlock(&lock_p[SHELDON]);
        pthread_mutex_unlock(&lock_p[HOWARD]);
        pthread_mutex_unlock(&lock_p[BERNADETTE]);
        pthread_mutex_unlock(&lock_p[PENNY]);
        pthread_mutex_unlock(&lock_p[AMY]);

        pthread_cond_wait(&cond_p[LEONARD], &lock_p[LEONARD]);
        pthread_mutex_unlock(&lock_p[LEONARD]);

        pthread_mutex_lock(&lock_p[SHELDON]);
        pthread_mutex_lock(&lock_p[HOWARD]);
        pthread_mutex_lock(&lock_p[LEONARD]);
        pthread_mutex_lock(&lock_p[BERNADETTE]);
        pthread_mutex_lock(&lock_p[PENNY]);
        pthread_mutex_lock(&lock_p[AMY]);
      }

      while(p[BERNADETTE].getQuer_usar()){
        pthread_mutex_unlock(&lock_p[SHELDON]);
        pthread_mutex_unlock(&lock_p[HOWARD]);
        pthread_mutex_unlock(&lock_p[LEONARD]);
        pthread_mutex_unlock(&lock_p[PENNY]);
        pthread_mutex_unlock(&lock_p[AMY]);

        pthread_cond_wait(&cond_p[BERNADETTE], &lock_p[BERNADETTE]);
        pthread_mutex_unlock(&lock_p[BERNADETTE]);

        pthread_mutex_lock(&lock_p[SHELDON]);
        pthread_mutex_lock(&lock_p[HOWARD]);
        pthread_mutex_lock(&lock_p[LEONARD]);
        pthread_mutex_lock(&lock_p[BERNADETTE]);
        pthread_mutex_lock(&lock_p[PENNY]);
        pthread_mutex_lock(&lock_p[AMY]);
      }

      while(p[PENNY].getQuer_usar()){
          pthread_mutex_unlock(&lock_p[SHELDON]);
          pthread_mutex_unlock(&lock_p[HOWARD]);
          pthread_mutex_unlock(&lock_p[LEONARD]);
          pthread_mutex_unlock(&lock_p[BERNADETTE]);
          pthread_mutex_unlock(&lock_p[AMY]);

          pthread_cond_wait(&cond_p[PENNY], &lock_p[PENNY]);
          pthread_mutex_unlock(&lock_p[PENNY]);

          pthread_mutex_lock(&lock_p[SHELDON]);
          pthread_mutex_lock(&lock_p[HOWARD]);
          pthread_mutex_lock(&lock_p[LEONARD]);
          pthread_mutex_lock(&lock_p[BERNADETTE]);
          pthread_mutex_lock(&lock_p[PENNY]);
          pthread_mutex_lock(&lock_p[AMY]);
        }

        while(p[AMY].getQuer_usar()){
          pthread_mutex_unlock(&lock_p[SHELDON]);
          pthread_mutex_unlock(&lock_p[HOWARD]);
          pthread_mutex_unlock(&lock_p[LEONARD]);
          pthread_mutex_unlock(&lock_p[BERNADETTE]);
          pthread_mutex_unlock(&lock_p[PENNY]);

          pthread_cond_wait(&cond_p[AMY], &lock_p[AMY]);
          pthread_mutex_unlock(&lock_p[AMY]);

          pthread_mutex_lock(&lock_p[SHELDON]);
          pthread_mutex_lock(&lock_p[HOWARD]);
          pthread_mutex_lock(&lock_p[LEONARD]);
          pthread_mutex_lock(&lock_p[BERNADETTE]);
          pthread_mutex_lock(&lock_p[PENNY]);
          pthread_mutex_lock(&lock_p[AMY]);
        }

        if(
          ((prior == KRIPKE) and (!p[SHELDON].getQuer_usar() and !p[HOWARD].getQuer_usar() and !p[LEONARD].getQuer_usar() and !p[BERNADETTE].getQuer_usar() and !p[PENNY].getQuer_usar() and !p[AMY].getQuer_usar() and !p[STUART].getQuer_usar()))
          or ((prior == STUART) and (!p[SHELDON].getQuer_usar() and !p[HOWARD].getQuer_usar() and !p[LEONARD].getQuer_usar() and !p[BERNADETTE].getQuer_usar() and !p[PENNY].getQuer_usar() and !p[AMY].getQuer_usar()))
        ){
          pthread_cond_signal(&cond_p[SHELDON]);
          pthread_mutex_unlock(&lock_p[SHELDON]);

          pthread_cond_signal(&cond_p[HOWARD]);
          pthread_mutex_unlock(&lock_p[HOWARD]);

          pthread_cond_signal(&cond_p[LEONARD]);
          pthread_mutex_unlock(&lock_p[LEONARD]);

          pthread_cond_signal(&cond_p[BERNADETTE]);
          pthread_mutex_unlock(&lock_p[BERNADETTE]);

          pthread_cond_signal(&cond_p[PENNY]);
          pthread_mutex_unlock(&lock_p[PENNY]);

          pthread_cond_signal(&cond_p[AMY]);
          pthread_mutex_unlock(&lock_p[AMY]);

          if(prior == KRIPKE){
            pthread_mutex_unlock(&lock_p[STUART]);
          }

          break;
        }      
        
    }
                                                              
  
  }
     
}

class Microwave {
	public:
		Microwave(){
    }

    void wait(Person &pessoa){
      pthread_mutex_lock(&lock_p[pessoa.getId()]);
      pessoa.setQuer_usar(true);

      /*int casal = casais(pessoa.getId());
      if(casal != -1){
        pthread_mutex_lock(&lock_p[casal]);

        if(p[casal].getQuer_usar()){
          int id_casal_junto = id_casal(casal);
          pthread_mutex_lock(&lock_casal_junto[id_casal_junto]);
          casal_junto[id_casal_junto] = true;
          pthread_mutex_unlock(&lock_casal_junto[id_casal_junto]);
        }

        pthread_mutex_unlock(&lock_p[casal]);
      }*/

      pthread_mutex_lock(&lock_cout);
      cout << pessoa.getName() << " quer usar o forno." << "\n";
      pthread_mutex_unlock(&lock_cout);

      int id_primeiro_casal = id_casal(pessoa.getId());
      if(id_primeiro_casal != -1){
        pthread_mutex_lock(&lock_primeiro_do_casal[id_primeiro_casal]);
        if(primeiro_do_casal[id_primeiro_casal] == -1)
          primeiro_do_casal[id_primeiro_casal] = pessoa.getId();
        pthread_mutex_unlock(&lock_primeiro_do_casal[id_primeiro_casal]);
      }    

      pthread_mutex_unlock(&lock_p[pessoa.getId()]);  

      trata_prioridades(pessoa);
    }

    void release(Person &pessoa){
      pthread_mutex_lock(&lock_p[pessoa.getId()]);
      pessoa.setQuer_usar(false);
      
      int id_casal_junto = id_casal(pessoa.getId());
      /*pthread_mutex_lock(&lock_casal_junto[id_casal_junto]);
      casal_junto[id_casal_junto] = false;
      pthread_mutex_unlock(&lock_casal_junto[id_casal_junto]);*/

      pthread_mutex_lock(&lock_primeiro_do_casal[id_casal_junto]);
      primeiro_do_casal[id_casal_junto] = -1;
      pthread_mutex_unlock(&lock_primeiro_do_casal[id_casal_junto]);

      pthread_cond_signal(&cond_p[pessoa.getId()]);
      pthread_mutex_unlock(&lock_p[pessoa.getId()]);

      pthread_mutex_unlock(&lock_forno);

      pthread_mutex_lock(&lock_cout);
      cout << pessoa.getName() << " vai comer." << "\n";
      pthread_mutex_unlock(&lock_cout);
    }

    void check(){
      bool deadlock = false;
      bool todos_casais = false;
      int pessoas_deadlock[9];
      srand48(time(NULL));
      int nmr_aleatorio;

      pthread_mutex_lock(&lock_p[SHELDON]);
      pthread_mutex_lock(&lock_p[HOWARD]);
      pthread_mutex_lock(&lock_p[LEONARD]);
      pthread_mutex_lock(&lock_p[BERNADETTE]);
      pthread_mutex_lock(&lock_p[PENNY]);
      pthread_mutex_lock(&lock_p[AMY]);

      if(p[SHELDON].getQuer_usar() and p[AMY].getQuer_usar() and p[HOWARD].getQuer_usar() and p[BERNADETTE].getQuer_usar() and p[LEONARD].getQuer_usar() and p[PENNY].getQuer_usar()){
        deadlock = true;
        todos_casais = true;        
      }
      else
      if(p[SHELDON].getQuer_usar() and p[HOWARD].getQuer_usar() and p[LEONARD].getQuer_usar()){
        pessoas_deadlock[0] = SHELDON;
        pessoas_deadlock[1] = HOWARD;
        pessoas_deadlock[2] = LEONARD;
        
        deadlock = true;
      }
      else
      if(p[SHELDON].getQuer_usar() and p[BERNADETTE].getQuer_usar() and p[LEONARD].getQuer_usar()){
        pessoas_deadlock[0] = SHELDON;
        pessoas_deadlock[1] = BERNADETTE;
        pessoas_deadlock[2] = LEONARD;
        
        deadlock = true;
      }
      else
      if(p[AMY].getQuer_usar() and p[BERNADETTE].getQuer_usar() and p[LEONARD].getQuer_usar()){
        pessoas_deadlock[0] = AMY;
        pessoas_deadlock[1] = BERNADETTE;
        pessoas_deadlock[2] = LEONARD;
        
        deadlock = true;
      }
      else
      if(p[AMY].getQuer_usar() and p[HOWARD].getQuer_usar() and p[LEONARD].getQuer_usar()){
        pessoas_deadlock[0] = AMY;
        pessoas_deadlock[1] = HOWARD;
        pessoas_deadlock[2] = LEONARD;
        
        deadlock = true;
      }
      else
      if(p[SHELDON].getQuer_usar() and p[HOWARD].getQuer_usar() and p[PENNY].getQuer_usar()){
        pessoas_deadlock[0] = SHELDON;
        pessoas_deadlock[1] = HOWARD;
        pessoas_deadlock[2] = PENNY;
        
        deadlock = true;
      }
      else
      if(p[SHELDON].getQuer_usar() and p[BERNADETTE].getQuer_usar() and p[PENNY].getQuer_usar()){
        pessoas_deadlock[0] = SHELDON;
        pessoas_deadlock[1] = BERNADETTE;
        pessoas_deadlock[2] = PENNY;
        
        deadlock = true;
      }
      else
      if(p[AMY].getQuer_usar() and p[BERNADETTE].getQuer_usar() and p[PENNY].getQuer_usar()){
        pessoas_deadlock[0] = AMY;
        pessoas_deadlock[1] = BERNADETTE;
        pessoas_deadlock[2] = PENNY;
        
        deadlock = true;
      }
      else
      if(p[AMY].getQuer_usar() and p[HOWARD].getQuer_usar() and p[PENNY].getQuer_usar()){
        pessoas_deadlock[0] = AMY;
        pessoas_deadlock[1] = HOWARD;
        pessoas_deadlock[2] = PENNY;
        
        deadlock = true;
      }

      //tratamento para casos de deadlock
      if(deadlock and !todos_casais){
        nmr_aleatorio = drand48 () * 2;
      }
      else
      if(deadlock and todos_casais){
        nmr_aleatorio = drand48 () * 8;
        if((nmr_aleatorio > 2) and (nmr_aleatorio < 6))
          nmr_aleatorio += 3;
      }

      if(deadlock){
        pthread_mutex_lock(&lock_raj_liberou);
        raj_liberou = true;
        pthread_mutex_unlock(&lock_raj_liberou); 
        pthread_cond_signal(&cond_p[pessoas_deadlock[nmr_aleatorio]]);
      }
      
      pthread_mutex_unlock(&lock_p[SHELDON]);
      pthread_mutex_unlock(&lock_p[HOWARD]);
      pthread_mutex_unlock(&lock_p[LEONARD]);
      pthread_mutex_unlock(&lock_p[BERNADETTE]);
      pthread_mutex_unlock(&lock_p[PENNY]);
      pthread_mutex_unlock(&lock_p[AMY]);
    }
};

 //MAIN

int threads_repeat;

Microwave forno;
int thrds_terminated = 0;
pthread_mutex_t lock_thrds_terminated;

void* execute_threads(void* thread) {
  long my_thread = (long) thread;

  if(my_thread != RAJ){
    for (int i = 0; i < threads_repeat; i++) {
      forno.wait(p[my_thread]);
      p[my_thread].cook_something();
      forno.release(p[my_thread]);
      p[my_thread].eat();
      p[my_thread].work();

      if(i == threads_repeat - 1){
        pthread_mutex_lock(&lock_thrds_terminated);
        thrds_terminated++;
        pthread_mutex_unlock(&lock_thrds_terminated);

        pthread_mutex_lock(&lock_cout);
        cout << "--------------------------------------" << "\n";
        cout << p[my_thread].getName() << ": Terminou suas tarefas." << "\n";
        pthread_mutex_unlock(&lock_cout);
        return NULL;
      }
    }
  }
  else{
    while(1){
      sleep(5);
      forno.check();

      pthread_mutex_lock(&lock_thrds_terminated);
      if(thrds_terminated == NTHREADS - 1){
        pthread_mutex_lock(&lock_cout);
        cout << "--------------------------------------" << "\n";
        cout << p[my_thread].getName() << ": Terminou suas tarefas." << "\n";
        pthread_mutex_unlock(&lock_cout);
        return NULL;
      }
      pthread_mutex_unlock(&lock_thrds_terminated);
    }
  }
  return NULL;
}

int main (int argc, char * argv []){

    int error;
    long thread;

    int i;
    for(i = 0; i < 10; i++){
      p[i].setId(i);
    }

    for(i = 0; i < 3; i++)
      primeiro_do_casal[i] = -1;

    for(i = 0; i < 3; i++)
      casal_junto[i] = false;

    pthread_t* thread_handles;

    threads_repeat = strtol(argv[1], NULL, 10);

    if (pthread_mutex_init(&lock_raj_liberou, NULL) != 0) {
      perror("\nMutex lock_raj_liberou init has failed");
      return 1;
    }

    if (pthread_mutex_init(&lock_cout, NULL) != 0) {
      perror("\nMutex lock_cout init has failed");
      return 1;
    }

    if (pthread_mutex_init(&lock_thrds_terminated, NULL) != 0) {
        perror("\nMutex lock_thrds_terminated init has failed");
        return 1;
    }

    if (pthread_mutex_init(&lock_forno, NULL) != 0) {
        perror("\nMutex lock_forno init has failed");
        return 1;
    }

    for(i = 0; i < NTHREADS; i++){

      if (pthread_cond_init(&cond_p[i], NULL) != 0) {
        perror("\nCond cond_p init has failed");
        return 1;
      }

      if (pthread_mutex_init(&lock_p[i], NULL) != 0) {
        perror("\nMutex lock_p init has failed");
        return 1;
      }
    }

    for(i= 0; i < 3; i++){
      if (pthread_mutex_init(&lock_primeiro_do_casal[i], NULL) != 0) {
        perror("\nMutex lock_primeiro_do_casal init has failed");
        return 1;
      }
    }

    for(i= 0; i < 3; i++){
      if (pthread_mutex_init(&lock_casal_junto[i], NULL) != 0) {
        perror("\nMutex lock_casal_junto init has failed");
        return 1;
      }
    }

    thread_handles = (pthread_t*) malloc (NTHREADS*sizeof(pthread_t));

    for (thread = 0; thread < NTHREADS; thread++){
      error = pthread_create(&thread_handles[thread], NULL, execute_threads, (void*) thread);
      if (error != 0)
        perror("\nThread can't be created");
    }

    for (thread = 0; thread < NTHREADS; thread++){
      pthread_join(thread_handles[thread], NULL);
    }

    free (thread_handles);
    for(i = 0; i < NTHREADS; i++){
      pthread_cond_destroy(&cond_p[i]);
      pthread_mutex_destroy(&lock_p[i]);
    }

    for(i= 0; i < 3; i++){
      pthread_mutex_destroy(&lock_primeiro_do_casal[i]);
      pthread_mutex_destroy(&lock_casal_junto[i]);
    }

    pthread_mutex_destroy(&lock_forno);
    pthread_mutex_destroy(&lock_thrds_terminated);
    pthread_mutex_destroy(&lock_raj_liberou);
    pthread_mutex_destroy(&lock_cout);
    return 0;
}
