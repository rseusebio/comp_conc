import java.util.Random;

class Writer extends Thread 
{
  int id; //identificador da thread
  Monitor monitor; //objeto monitor para coordenar a lógica de execução das threads
  Var var;
  String type;
  int executions;

  // Construtor
  Writer (int id, Monitor m, Var v, int e) 
  {
    this.id = id;
    this.monitor = m;
    this.var = v;
    this.type = "Writer";
    this.executions = e;
  }

  public void execute()
  {
    this.monitor.EntraEscritor(this.id, this.type); 
    
    this.var.set(this.id);

    System.out.println("Writer " + id + " set var value to " + this.var.get());

    this.monitor.SaiEscritor(this.id, this.type);
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