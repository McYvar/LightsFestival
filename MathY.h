class MathY 
{
public:
  static float Lerp(float a, float b, float t) 
  {
    return (1.0f - t) * a + b * t;
  }

  static float InvLerp(float a, float b, float t) 
  {
    return (t - a) / (b - a);
  }
};