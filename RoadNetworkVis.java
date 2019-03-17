import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
import java.io.*;
import java.security.*;
import java.util.*;
import javax.swing.*;


public class RoadNetworkVis {

    private static int minN = 30, maxN = 1000;           
    private static int minP = 1, maxP = 5;      
    private static int H = 1000, W = 1600;
    private static int extraH = 50, extraW = 250;
    private static int minRoutes = 5;
    
    
    //visuals
    private static int Height = 800, Width = 1280;
    private static int MinCityDist = 20;
    private static int UnitDist = 50;
    private static int CitySize=10;
    private static int EdgeGap=15;
    private int[] CityX, CityY;
    
    //extra
    int RoutesCompleted=0;
    int RoutesFailed=0;
    int ScoreCompleted=0;
    int ScoreFailed=0;
    int RouteScore=0;
    int EdgeScore=0;
    int MaterialsUsed=0;
    long SeedValue;

    //inputs
    int N;      //num cities
    int NumRoutes;      //number of tickets to choose from
    int NumMaterials;   //number of building materials
    java.util.List<Edge> Edges;
    Edge[] Routes;
    
        
    // -----------------------------------------
    void generate(String seedStr) {
      try {
        // generate test case
        SecureRandom r1 = SecureRandom.getInstance("SHA1PRNG"); 
        long seed = Long.parseLong(seedStr);
        r1.setSeed(seed);
        SeedValue = seed;

        N = r1.nextInt(maxN - minN + 1) + minN;

        if (seed == 1)
        {
          N=minN;
        }      
        if (seed == 2)
        {
          N=maxN;
        }

        //generate cities
        CityX = new int[N];
        CityY = new int[N];
        for (int i=0; i<N; )
        {
          CityX[i]=r1.nextInt(W-2*EdgeGap)+EdgeGap;
          CityY[i]=r1.nextInt(H-2*EdgeGap)+EdgeGap;

          //check that city is not close to others
          boolean ok=true;
          for (int k=0; k<i; k++)
            if (distance(i,k)<MinCityDist)
            {
                ok=false;
                break;
            }

          if (ok) i++;
        }

        //generate edges
        Edges=new ArrayList<Edge>();
        int components=N;
        int[] parent=new int[N];
        for (int i=0; i<N; i++) parent[i]=i;
        
        while(components>1)
        {
          int a=r1.nextInt(N);

          java.util.List<Point> points=new ArrayList<Point>();
          for (int i=0; i<N; i++)
          {
            if (a==i) continue;
            points.add(new Point(i,i,distance(i,a)));
          }
          Collections.sort(points);

loop:          
          for (Point p : points)
          {
            int b=p.x;
            
            //check that edge doesn't intersect cities
            for (int i=0; i<N; i++)
              if (i!=a && i!=b && intersectCircle(a,b,i))
                continue loop;

            //check that edge doesn't intersect other edges
            for (Edge e : Edges)
              if (intersectLines(a,b,e.a,e.b))
                continue loop;

                
            //System.err.println(a+" "+b);
            int d=unitDistance(distance(a,b));
            int edgePoints=(r1.nextInt(maxP-minP+1)+minP)*d;
            
            Edges.add(new Edge(a,b,d,edgePoints));
            if (parent[a]!=parent[b])
            {
              components--;
              int t=parent[b];
              for (int k=0; k<N; k++) if (parent[k]==t) parent[k]=parent[a];              
            }
            break;
          }
        }

        
        //generate routes
        int bonusPoints=0;
        NumRoutes = r1.nextInt(N/4 - minRoutes + 1) + minRoutes;        
        Routes=new Edge[NumRoutes];
        Set<String> seen=new HashSet<String>();
        
        for (int i=0; i<NumRoutes; )
        {
          int a=r1.nextInt(N);          
          int b=r1.nextInt(N);          
          if (a==b) continue;
          
          String h=Math.min(a,b)+" "+Math.max(a,b);
          if (seen.contains(h)) continue;
          
          seen.add(h);
          int d=unitDistance(distance(a,b));
          bonusPoints+=d;
          Routes[i]=new Edge(a,b,d,d);
          i++;
        }
        
        
        NumMaterials=r1.nextInt(bonusPoints/4-bonusPoints/8+1) + bonusPoints/8;
        
        if (Debug)
        {
          System.out.println("N "+N+" E "+Edges.size()+" numMaterials "+NumMaterials+" numRoutes "+NumRoutes);
          System.out.println("Edges: city1 city2 materialsNeeded points");
          for (Edge r : Edges) System.out.println(r.a+" "+r.b+" "+r.dist+" "+r.points);        
          System.out.println("Routes: city1 city2 points");
          for (Edge r : Routes) System.out.println(r.a+" "+r.b+" "+r.points);
        }
      }
      catch (Exception e) {
        addFatalError("An exception occurred while generating test case.");
        e.printStackTrace(); 
      }
    }
    
    int unitDistance(double dist)
    {
      int d=(int)Math.floor(dist/UnitDist); 
      return Math.max(d,1);
    }

    double distance(int i, int k)
    {
      return distance(CityX[i],CityY[i],CityX[k],CityY[k]);
    }

    double distance(int x1, int y1, int x2, int y2)
    {
      return Math.sqrt(sq(x1-x2)+sq(y1-y2));
    }

    int sq(int a)
    {
      return a*a;
    }


    boolean intersectLines(int A, int B, int C, int D)
    {
      return (intersectLines2(A,B,C,D) && intersectLines2(C,D,A,B));
    }

    //based on https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
    boolean intersectLines2(int A, int B, int C, int D)
    {
      if ((A==C && B==D) || (A==D && B==C)) return true;

      int Ex=CityX[B]-CityX[A];
      int Ey=CityY[B]-CityY[A];
      int Fx=CityX[D]-CityX[C];      
      int Fy=CityY[D]-CityY[C];   
      int Gx=CityX[A]-CityX[C];
      int Gy=CityY[A]-CityY[C];      

      int top=dot(Gx,Gy,-Ey,Ex);
      int bottom=dot(Fx,Fy,-Ey,Ex);
      if (bottom==0) return false;

      double d=top*1.0/bottom;
      return (d>0 && d<1);
    }
    
    
    //check if segment A-B intersects point C
    boolean intersectCircle(int A, int B, int C)
    {
      return intersectCircle2(CityX[A],CityY[A],CityX[B],CityY[B],CityX[C],CityY[C]);
    }

    //p1 and p2 are points on the segment, p3 is the circle center
    //based on http://paulbourke.net/geometry/pointlineplane/DistancePoint.java
    boolean intersectCircle2(int x1, int y1, int x2, int y2, int x3, int y3)
    {
      int xDelta = x2-x1;
      int yDelta = y2-y1;
      double u = ((x3-x1) * xDelta + (y3-y1) * yDelta) * 1.0 / (sq(xDelta) + sq(yDelta));

      Point closestPoint;
      if (u < 0) closestPoint = new Point(x1,y1,0);
      else if (u > 1) closestPoint = new Point(x2,y2,0);
      else closestPoint = new Point((int)Math.round(x1 + u * xDelta), (int)Math.round(y1 + u * yDelta),0);
      
      //System.err.println(distance(closestPoint.x,closestPoint.y,x3,y3));

      return (distance(closestPoint.x,closestPoint.y,x3,y3) < CitySize);
    }    


    int dot(int x1, int y1, int x2, int y2)
    {
      return x1*x2+y1*y2;
    }
    

    int routeScore()
    {
      //stores whether i and j have become connected
      boolean[][] adj=new boolean[N][N];
      
      for (int i : Ret)
      {
        Edge e=Edges.get(i);
        adj[e.a][e.b]=true;
        adj[e.b][e.a]=true;
      }
      
              
      //construct connectivity map
      for (int k=0; k<N; k++)
        for (int i=0; i<N; i++)
          if (adj[i][k])
            for (int j=0; j<N; j++)
              if (adj[k][j])
                adj[i][j]=true;              

      
      RoutesCompleted=0;
      ScoreCompleted=0;
      RoutesFailed=0;
      ScoreFailed=0;
      for (Edge r : Routes)
      {
        if (adj[r.a][r.b])
        {
          RoutesCompleted++;
          ScoreCompleted+=r.points;
        }
//         else
//         {
//           RoutesFailed++;
//           ScoreFailed+=r.points;
//         }
      }
      
      //System.err.println("routes completed "+routesCompleted);
      return ScoreCompleted-ScoreFailed;
    }
        
    
    int edgeScore()
    {
      int score=0;
      for (int i : Ret)
      {
        Edge e=Edges.get(i);
        score+=e.points;
      }
      return score;
    }
    
    
    // -----------------------------------------
    public double runTest(String seed) {
      try {
        generate(seed);

        if (proc != null) {
            // call the solution
            int[] ret;
            try { 
                ret = callSolution();
            } catch (Exception e) {
                addFatalError("Failed to get result from your solution.");
                return -1;
            }

            // check the return and score it
            if (ret == null) {
                addFatalError("Your return contained invalid number of elements.");
                return -1;
            }
            
            if (ret.length > Edges.size())
            {
                addFatalError("Your return contains too many elements.");
                return -1;            
            }
            
            boolean[] edgeSeen=new boolean[Edges.size()];
            for (int i : ret)
            {
              if (i<0 || i>=Edges.size())
              {
                addFatalError("Edge "+i+" does not exist.");
                return -1;                              
              }              
              if (edgeSeen[i])
              {
                addFatalError("Your return contained the same edge "+i+" multiple times");
                return -1;                
              }
              edgeSeen[i]=true;
              
              Edge e=Edges.get(i);
              MaterialsUsed+=e.dist;
            }
            if (MaterialsUsed>NumMaterials)
            {
              addFatalError("You used more materials than there is available");
              return -1;                            
            }
            
                                 
            
            Ret=ret;
            
            if (vis) {
                jf.setSize(Width+extraW,Height+extraH);
                jf.setVisible(true);
                draw();
            }   
        }
       
        RouteScore=routeScore();
        EdgeScore=edgeScore();
        return RouteScore*1.0*EdgeScore;
      }
      catch (Exception e) { 
        addFatalError("An exception occurred while trying to get your program's results.");
        e.printStackTrace(); 
        return -1;
      }
    }
// ------------- visualization part ------------
    JFrame jf;
    Vis v;
    static String exec;
    static boolean vis;
    static boolean showNumbers;
    static boolean Debug;
    static boolean json;
    static Process proc;
    InputStream is;
    OutputStream os;
    BufferedReader br;
    static int[] Ret;
    
    
    
    // -----------------------------------------
    private int[] callSolution() throws IOException, NumberFormatException {
      if (exec == null) return null;

      os.write((NumMaterials+"\n").getBytes());
      os.write((N+"\n").getBytes());
      os.write((Edges.size()+"\n").getBytes());
      for (Edge e : Edges)
        os.write((e.a+" "+e.b+" "+e.dist+" "+e.points+"\n").getBytes());
      os.write((NumRoutes+"\n").getBytes());
      for (Edge e : Routes)
        os.write((e.a+" "+e.b+" "+e.points+"\n").getBytes());      
      os.flush();
      
      int[] ret=new int[Integer.parseInt(br.readLine())];
      for (int i=0; i<ret.length; i++)
        ret[i]=Integer.parseInt(br.readLine());
      return ret;
    }
    // -----------------------------------------
    void draw() {
        if (!vis) return;
        v.repaint();
    }
    // -----------------------------------------
    public class Vis extends JPanel implements MouseListener, WindowListener {
        public void paint(Graphics g) {
            
            // background
            g.setColor(new Color(0xDDDDDD));
            g.fillRect(0,0,Width+extraW,Height+extraH);
            g.setColor(Color.WHITE);
            g.fillRect(0,0,Width,Height);

            double ratioX=1.0/W*Width;
            double ratioY=1.0/H*Height;
            
            
            boolean[] isInRoute=new boolean[N];
            for (Edge r : Routes)
            {
              isInRoute[r.a]=true;
              isInRoute[r.b]=true;
            }
            
            boolean[] edgeInReturn=new boolean[Edges.size()];
            for (int i : Ret) edgeInReturn[i]=true;
           
            //edges
            Graphics2D g2 = (Graphics2D) g.create();
            for (int i=0; i<Edges.size(); i++)
            {
              Edge e=Edges.get(i);
              
              if (edgeInReturn[i]) g2.setColor(Color.GREEN);
              else g2.setColor(Color.BLUE);
              
              double dist=distance(e.a,e.b);
              float gap=10;
              float X=(float)((dist-(e.dist-1)*gap)/e.dist);
              Stroke dashed = new BasicStroke(2, BasicStroke.CAP_BUTT, BasicStroke.JOIN_BEVEL, 0, new float[]{X,gap}, 0);
              g2.setStroke(dashed);              
              g2.drawLine((int)(CityX[e.a]*ratioX),(int)(CityY[e.a]*ratioY),(int)(CityX[e.b]*ratioX),(int)(CityY[e.b]*ratioY));

              if (showNumbers)
              {
                g.setColor(Color.BLACK);
                g.setFont(new Font("Arial",Font.PLAIN,10));
                g.drawString(""+e.points,(int)((CityX[e.a]+CityX[e.b])/2*ratioX),(int)((CityY[e.a]+CityY[e.b])/2*ratioY));
              }
            }            
            g2.dispose();
            
            //cities
            for (int i=0; i<N; i++)
            {
              if (isInRoute[i]) g.setColor(Color.GREEN);
              else g.setColor(Color.RED);
              
              g.fillOval((int)((CityX[i]-CitySize/2)*ratioX),(int)((CityY[i]-CitySize/2)*ratioY),(int)(CitySize*ratioX),(int)(CitySize*ratioY));
              
              if (showNumbers)
              {
                g.setColor(Color.RED);
                g.setFont(new Font("Arial",Font.PLAIN,10));
                g.drawString(""+i,(int)((CityX[i]-CitySize/2)*ratioX),(int)((CityY[i]-CitySize/2)*ratioY));
              }
            }            
            
            //show stats
            g.setColor(Color.BLACK);
            g.setFont(new Font("Arial",Font.BOLD,14));
            g.drawString(String.format("Routes completed: %d", RoutesCompleted), Width+25, 30);
            //g.drawString(String.format("Routes failed: %d", RoutesFailed), Width+25, 60);
            g.drawString(String.format("Routes score: %d", RouteScore), Width+25, 90);
            g.drawString(String.format("Connections score: %d", EdgeScore), Width+25, 120);
            g.drawString(String.format("SCORE: %d", RouteScore*1L*EdgeScore), Width+25, 150);
        }
        // -------------------------------------
        public Vis() {
          jf.addWindowListener(this); 
        }
        // -------------------------------------
        //WindowListener
        public void windowClosing(WindowEvent e){ 
            if(proc != null)
                try { proc.destroy(); } 
                catch (Exception ex) { ex.printStackTrace(); }
            System.exit(0); 
        }
        public void windowActivated(WindowEvent e) { }
        public void windowDeactivated(WindowEvent e) { }
        public void windowOpened(WindowEvent e) { }
        public void windowClosed(WindowEvent e) { }
        public void windowIconified(WindowEvent e) { }
        public void windowDeiconified(WindowEvent e) { }
        // -------------------------------------
        //MouseListener
        public void mouseClicked(MouseEvent e) { }
        public void mousePressed(MouseEvent e) { }
        public void mouseReleased(MouseEvent e) { }
        public void mouseEntered(MouseEvent e) { }
        public void mouseExited(MouseEvent e) { }
    }
    // -----------------------------------------
    public RoadNetworkVis(String seed) {
      try {
        if (vis)
        {   jf = new JFrame();
            v = new Vis();
            jf.getContentPane().add(v);
        }
        if (exec != null) {
            try {
                Runtime rt = Runtime.getRuntime();
                proc = rt.exec(exec);
                os = proc.getOutputStream();
                is = proc.getInputStream();
                br = new BufferedReader(new InputStreamReader(is));
                new ErrorReader(proc.getErrorStream()).start();
            } catch (Exception e) { e.printStackTrace(); }
        }
        double score = runTest(seed);
        if (!json) System.out.println("Score = " + score);
        if (proc != null)
            try { proc.destroy(); } 
            catch (Exception e) { e.printStackTrace(); }

        if (json)
        {
          System.out.print("{\"seed\":"+SeedValue);
          System.out.print(",\"NM\":"+NumMaterials);
          System.out.print(",\"N\":"+N);
          System.out.print(",\"E\":"+Edges.size());
          System.out.print(",\"R\":"+NumRoutes);
          System.out.print(",\"edges\":[");
          String sep = "";
          for (Edge r : Edges) {
            System.out.print(sep+"["+r.a+","+r.b+","+r.dist+","+r.points+"]");
            sep = ",";
          }
          System.out.print("]");
          System.out.print(",\"routes\":[");
          sep = "";
          for (Edge r : Routes) {
            System.out.print(sep+"["+r.a+","+r.b+","+r.points+"]");
            sep = ",";
          }
          System.out.print("]");
          System.out.print(",\"return\":[");
          sep = "";
          for (int ret : Ret) {
            System.out.print(sep+ret);
            sep = ",";
          }
          System.out.print("]");
          System.out.print(",\"score\":"+score);
          System.out.print(",\"details\":");
          System.out.print("{\"materialsUsed\":"+MaterialsUsed);
          System.out.print(",\"routesCompleted\":"+RoutesCompleted);
          System.out.print(",\"routeScore\":"+RouteScore);
          System.out.print(",\"edgeScore\":"+EdgeScore);
          System.out.print("}");
          System.out.println("}");
        }
      }
      catch (Exception e) { e.printStackTrace(); }
    }
    
    class Point implements Comparable<Point>
    {
      int x;
      int y;
      double dist;

      public Point(int X, int Y, double d)
      {
        x=X;
        y=Y;
        dist=d;
      }


      //shortest to longest
      public int compareTo(Point d)
      {
        if (dist<d.dist) return -1;
        if (dist>d.dist) return +1;
        return 0;
      }
    }


    class Edge
    {
      int a;
      int b;
      int dist;
      int points;

      public Edge(int A, int B, int d, int p)
      {
        a=A;
        b=B;
        dist=d;
        points=p;
      }
    }
    
    
    // -----------------------------------------
    public static void main(String[] args) {
        String seed = "1";
        vis = true;
        showNumbers = false;
        Debug = false;
        json = false;
        for (int i = 0; i<args.length; i++)
        {   if (args[i].equals("-seed"))
                seed = args[++i];
            if (args[i].equals("-exec"))
                exec = args[++i];
            if (args[i].equals("-novis"))
                vis = false;
            if (args[i].equals("-showNumbers"))
                showNumbers = true;                
            if (args[i].equals("-debug"))
                Debug = true;                                
            if (args[i].equals("-json"))
                json = true;
            if (args[i].equals("-width"))
                Width = Integer.parseInt(args[++i]);
            if (args[i].equals("-height"))
                Height = Integer.parseInt(args[++i]);              
        }
            
        RoadNetworkVis f = new RoadNetworkVis(seed);
    }
    // -----------------------------------------
    void addFatalError(String message) {
        System.out.println(message);
    }
}

class ErrorReader extends Thread{
    InputStream error;
    public ErrorReader(InputStream is) {
        error = is;
    }
    public void run() {
        try {
            byte[] ch = new byte[50000];
            int read;
            while ((read = error.read(ch)) > 0)
            {   String s = new String(ch,0,read);
                System.err.print(s);
                System.err.flush();
            }
        } catch(Exception e) { }
    }
}
