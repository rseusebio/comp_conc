public class Leitor extends Thread 
{
    int id; //identificador da thread
    int delay; //atraso bobo
    Monitor monitor;//objeto monitor para coordenar a lógica de execução das threads
  
    // Construtor
    Leitor (int id, int delayTime, Monitor m) 
    {
      this.id = id;
      this.delay = delayTime;
      this.monitor = m;
    }
  
    // Método executado pela thread
    public void run () 
    {
      try 
      {
        while (true) 
        {
          this.monitor.EntraLeitor(this.id);

          new Silly().go();

          System.out.println("L" + this.id + " leu");

          this.monitor.SaiLeitor(this.id);

          sleep(this.delay);
        }
      } 
      catch (InterruptedException e) { return; }
    }
  }