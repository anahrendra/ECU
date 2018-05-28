ISR( TIMER4_COMPA_vect )
{
  // No rotation in the last 500 ms
  if (lastTemp == temp1) {
    isEngineIdle = 0;
    isStarting = 0;
    startingCount = 0;
  }
  // Engine is rotating
  else{
    
    if(isEngineIdle == 0){
      isStarting = 1;
      startingCount++;

      // If engine is rotating for more than 10 * 500 ms
      // Update engine status
      if(startingCount >= 3){
        isEngineIdle = 1;
        isStarting = 0;
      }    
    }
  }
  
  // Update time stamp
  lastTemp = temp1;
}
