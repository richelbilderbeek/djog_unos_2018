class Rectangle {
  public float xb;
  public float yb;
  public float xe;
  public float ye;
}


boolean check_if_two_rectangles_hit(Rectangle a, Rectangle b) {
  float bx[] = {b.xb, b.xe, b.xb, b.xe};
  float by[] = {b.yb, b.yb, b.ye, b.ye};
  for (int i = 0; i < 4; ++i) {
    if ( a.xb <= bx[i] && bx[i] <= a.xe &&
      a.yb <= by[i] && by[i] <= a.ye) {
      return true;
    }
  }
  return false;
}

class Rocket {
  PImage photo;
  public float x;
  public float y;
  public int direction;
  public int bulletWait;
  public boolean firePressed;

  public Rocket(float x, float y) {
    photo = loadImage("held.jpg");
    this.x = x;
    this.y = y - 50;
  }

  private boolean canFire() {
    return (!firePressed && bulletWait == 0);
  }

  private void fire() {
    bullets.add(new Bullet(rocket.x + 75, rocket.y));
    this.bulletWait = 1;
  }

  public void pressFire() {
    if (rocket.canFire()) {
      rocket.fire();
      this.firePressed = true;
    }
  }
  public void releaseFire() {
    this.firePressed = false;
  }

  public void setDirection(int direction) {
    this.direction = direction;
  }

  public void update() {
    imageMode(CORNER);
    image(photo, x, y, 150, 150);
    x += 5 * direction;
    if (this.bulletWait > 0) {
      this.bulletWait -= 1;
    }
  }
  public void update2() {
    for (int i = bullets.size() - 1; i >= 0; --i) {
      if (!bullets.get(i).update()) {
        bullets.remove(i);
      }
    }
  }
}

class Bullet {
  public float x;
  public float y;

  public Bullet(float x, float y) {
    this.x = x;
    this.y = y;
  }
  public String toString() {
    return "Bullet(" + nf(x) + ", " + nf(y) + ")";
  }

  public Rectangle getRect() {
    Rectangle r = new Rectangle();
    r.xb = x + 2;
    r.yb = y;
    r.xe = x + 4;
    r.ye = y + 20;
    return r;
  }

  public boolean update() {
    stroke(255, 0, 0);
    fill(255, 0, 0);
    rect(x + 2, y, 2, 20);
    y -= 7.5;
    if (y < 0) {
      return false;
    }
    return true;
  }
}
class Tegenstander {
  PImage tegenstander;
  public float x1;
  public float y1;
  public boolean geraakt;

  public Tegenstander(float x1, float y1) {
    if (geraakt == false) {
      tegenstander = loadImage("tegenstander.png");
    }
    this.x1 = x1;
    this.y1 = y1;
    geraakt = false;
  }

  public Rectangle getRect() {
    Rectangle r = new Rectangle();
    r.xb = x1;
    r.yb = y1;
    r.xe = x1 + 80;
    r.ye = y1 + 50;
    return r;
  }
  public void update() {
    if (geraakt)
      return;
    imageMode(CORNER);
    image(tegenstander, x1, y1, 80, 80);
    x1 += random(-20, 20);
    if (x1 >= width) {
      x1 = 0;
    } else if (x1 <= 0) {
      x1 = width;
    }
  }
  class Tegenstanders2 {
    public float x;
    public float y;

    public String toString() {
      return "Punt(" + nf(x) + ", " + nf(y) + ")";
    }
    public void meer_tegenstanders() {
      float[] ts;
      for (int tegen=0; 1<4; ++tegen) {
        ts[tegen] = 0;
      }
    }
  }


  public boolean check_if_hit(Rectangle r) {
    return check_if_two_rectangles_hit(this.getRect(), r);
  }

  public void check_of_schip_is_geraakt(ArrayList<Bullet> bullets_b) {
    for (int a = 0; a < 25; a++) {
      for (int i = bullets_b.size() - 1; i >= 0; --i) {
        Bullet b_p = bullets_b.get(i);
        if (check_if_hit(b_p.getRect())) {
          geraakt = true;
        }
      }
    }
  }
}


Rocket rocket = null;
Tegenstander tegenstander =null;
ArrayList<Bullet> bullets = new ArrayList<Bullet>();

void setup()
{
  fullScreen(); 
  rocket = new Rocket(width / 2, height - 100);
  tegenstander = new Tegenstander(width / 2, 200);
}

void draw()
{
  background(255);
  tegenstander.update();
  rocket.update2();
  rocket.update();
  tegenstander.check_of_schip_is_geraakt(bullets);
}

void keyPressed() {
  int key_ = (int)key;
  if (key == CODED)
    key_ = keyCode;
  if (key_ == LEFT) rocket.setDirection(-1);
  else if (key_ == RIGHT) rocket.setDirection(+1);
  else if (key_ == UP) rocket.pressFire();
}

void keyReleased() {
  int key_ = (int)key;
  if (key == CODED)
    key_ = keyCode;
  if (key_ == LEFT || key_ == RIGHT) rocket.setDirection(0);
  else if (key_ == UP) rocket.releaseFire();
}
