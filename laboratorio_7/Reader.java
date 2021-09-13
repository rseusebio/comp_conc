import java.util.Random;

class Reader extends Thread 
{
  int id; //identificador da thread
  Monitor monitor;//objeto monitor para coordenar a lógica de execução das threads
  Var var;
  String type;
  int executions;

  // Construtor
  Reader (int id, Monitor m, Var v, int e) 
  {
    this.id = id;
    this.monitor = m;
    this.var = v;
    this.type = "Reader";
    this.executions = e;
  }

  public void isPrime(int x)
  {
    boolean isItPrime = true;

    if (x == 0 || x == 1)
    {
      System.out.println(type + " " + id + " says that " + x + " is NOT Prime");

      return;
    }

    for(int i = 2; i <= x/2; i++)
    {
      if (x % i == 0)
      {
        System.out.println(type + " " + id + " says that " + x + " is NOT Prime");

        isItPrime = false;

        return;
      }
    }

    if(isItPrime)
    {
      System.out.println(type + " " + id + " says that " + x + " IS Prime");
    }
  }

  public void execute()
  {
    this.monitor.EntraLeitor(this.id, this.type);

    int x = this.var.get();

    this.isPrime(x);

    this.monitor.SaiLeitor(this.id, this.type);
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