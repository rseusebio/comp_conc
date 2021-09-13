import java.util.Random;

class Hybrid extends Thread 
{
  int id; //identificador da thread
  Monitor monitor; //objeto monitor para coordenar a lógica de execução das threads
  Var var;
  String type;
  int executions;

  // Construtor
  Hybrid (int id, Monitor m, Var v, int e) 
  {
    this.id = id;
    this.monitor = m;
    this.var = v;
    this.type = "Hybrid";
    this.executions = e;
  }

  public void EvenOrOdd(int x)
  {
    if (x % 2 == 0)
    {
      System.out.println(type + " " + id + " says that " + x + " is EVEN.");
    }
    else
    {
      System.out.println(type + " " + id + " says that " + x + " is ODD.");
    }
  }

  public void execute()
  {
    this.monitor.EntraLeitor(this.id, this.type); 
    
    int x = this.var.get();

    EvenOrOdd(x);

    this.monitor.SaiLeitor(id, type); 

    this.monitor.EntraEscritor(id, type);

    int n = this.var.get() * 2;

    this.var.set(n);

    System.out.println(type + " " + id + " set var value to " + this.var.get());
    
    this.monitor.SaiEscritor(id, type);
  }

  // Método executado pela thread
  public void run () 
  { 
    Random rand = new Random();

    for(int i = 0; i < executions; i++)
    {
      execute();

      try 
      {
        long miliseconds = rand.nextInt(400) + 100; 

        Thread.sleep(miliseconds);
      }
      catch(InterruptedException ex) {}
    }
  }
}
