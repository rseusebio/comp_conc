/**
 * Essa classe serve como um contador 
 * de quantidade de blocos que ainda 
 * faltam para serem ordenados.
 */
public class BlockCount 
{
  private int unsortedBlocks;
  
  BlockCount(int blocks) 
  { 
     this.unsortedBlocks = blocks;
  } 
  
  public synchronized void Decrement()
  {
    this.unsortedBlocks--;
  }
  
  public synchronized boolean isItDone()  
  {
    return unsortedBlocks == 0;
  }
}
