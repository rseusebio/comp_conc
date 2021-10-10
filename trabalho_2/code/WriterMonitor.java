/**
 * Monitor utilizado para 
 * fazer a sincronização de escrita entre 
 * as Threads Consumidoras
 * OBS: Poderia ter sido facilmente substituido por
 * uma instãncia de um Semaphore.
 */
class WriterMonitor 
{
  private int escr;
  
  WriterMonitor() 
  { 
     this.escr = 0;
  } 
  
  public synchronized void Enter(int id)
  {
    try 
    { 
      while (this.escr > 0) 
      {
         wait();
      }

      this.escr++;

    } 
    catch (InterruptedException e) { }
  }
  
  public synchronized void Leave(int id)  
  {
     this.escr--;

     notify();
  }
}
