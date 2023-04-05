class Timer
{
public:
  Timer();
  ~Timer();
  void Update(unsigned long currentTime);
  unsigned long lastFrameTime;
  unsigned long deltaTime;
};