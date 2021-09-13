/* Disciplina: Computacao Concorrente */
/* Aluno.: Rafael Santos Eusébio */
/* DRE: 117208725 */
/* Codigo: Leitores e escritores usando monitores em Java */
/* -------------------------------------------------------------------*/

//--------------------------------------------------------
// Classe principal
class Main 
{
  static final int L = 3;
  static final int E = 3;
  static final int H = 3;
  static final int execs = 5;

  public static void main (String[] args) 
  {
    int i;
    
    Monitor monitor = new Monitor();            // Monitor (objeto compartilhado entre leitores e escritores)
    Var var = new Var();
    
    Reader[] l = new Reader[L];       // Threads leitores
    Writer[] e = new Writer[E];   // Threads escritores
    Hybrid[] h = new Hybrid[H];   // Threads escritores
    
    for (i = 0; i < L; i++)    
    {
       l[i] = new Reader(i+1, monitor, var, execs);

       l[i].start(); 
    }

    for (i = 0; i < E; i++)    
    {
       e[i] = new Writer(i+1, monitor, var, execs);

       e[i].start(); 
    }

    for (i = 0; i < H; i++)    
    {
       h[i] = new Hybrid(i+1, monitor, var, execs);

       h[i].start(); 
    }

    for (i = 0; i < L; i++)    
    {
      try 
      {
        l[i].join(); 
      }
      catch (InterruptedException ex) {}
    }

    for (i = 0; i < E; i++)    
    {
      try 
      {
        e[i].join(); 
      }
      catch (InterruptedException ex) {}
    }

    for (i = 0; i < H; i++)    
    {
      try 
      {
        h[i].join(); 
      }
      catch (InterruptedException ex) {}
    }

  }
}
