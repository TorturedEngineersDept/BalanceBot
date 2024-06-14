class navigationControl(Node):
    def __init__(self):
        super().__init__('Exploration')
        self.subscription = self.create_subscription(OccupancyGrid,'map',self.map_callback,10)
        self.subscription = self.create_subscription(Odometry,'odom_rf2o',self.odom_callback,10)
        self.subscription = self.create_subscription(LaserScan,'scan',self.scan_callback,10)
        self.publisher = self.create_publisher(Twist, 'cmd_vel', 10)
        print("[Note] Exploration mode activated")
        self.kesif = True
        self.exploration_completed = False  # Add a flag to track exploration completion
        threading.Thread(target=self.exp).start()

    def exp(self):
        twist = Twist()
        while True:
            if not hasattr(self,'map_data') or not hasattr(self,'odom_data') or not hasattr(self,'scan_data'):
                time.sleep(0.1)
                continue
            if self.kesif == True:
                if isinstance(pathGlobal, int) and pathGlobal == 0:
                    column = int((self.x - self.originX)/self.resolution)
                    row = int((self.y- self.originY)/self.resolution)
                    exploration(self.data,self.width,self.height,self.resolution,column,row,self.originX,self.originY)
                    self.path = pathGlobal
                else:
                    self.path = pathGlobal
                if isinstance(self.path, int) and self.path == -1:
                    print("[Note] Exploration completed")
                    self.exploration_completed = True  # Set the flag to indicate exploration completion
                    sys.exit()
                self.c = int((self.path[-1][0] - self.originX)/self.resolution) 
                self.r = int((self.path[-1][1] - self.originY)/self.resolution) 
                self.kesif = False
                self.i = 0
                print("[Note] A new target has been determined")
                t = pathLength(self.path)/speed
                t = t - 0.2
                self.t = threading.Timer(t,self.target_callback)
                self.t.start()
            
            else:
                v , w = localControl(self.scan)
                if v == None:
                    v, w,self.i = pure_pursuit(self.x,self.y,self.yaw,self.path,self.i)
                if(abs(self.x - self.path[-1][0]) < target_error and abs(self.y - self.path[-1][1]) < target_error):
                    v = 0.0
                    w = 0.0
                    self.kesif = True
                    print("[Note] Goal achieved")
                    self.t.join()
                twist.linear.x = v
                twist.angular.z = w
                self.publisher.publish(twist)
                time.sleep(0.1)

    def target_callback(self):
        if not self.exploration_completed:  # Only perform exploration if it has not been completed
            exploration(self.data,self.width,self.height,self.resolution,self.c,self.r,self.originX,self.originY)
        
    def scan_callback(self,msg):
        self.scan_data = msg
        self.scan = msg.ranges

    def map_callback(self,msg):
        self.map_data = msg
        self.resolution = self.map_data.info.resolution
        self.originX = self.map_data.info.origin.position.x
        self.originY = self.map_data.info.origin.position.y
        self.width = self.map_data.info.width
        self.height = self.map_data.info.height
        self.data = self.map_data.data
        print(f"[INFO] Map received: width={self.width}, height={self.height}, resolution={self.resolution}")

    def odom_callback(self,msg):
        self.odom_data = msg
        self.x = msg.pose.pose.position.x
        self.y = msg.pose.pose.position.y
        self.yaw = euler_from_quaternion(msg.pose.pose.orientation.x,msg.pose.pose.orientation.y,
        msg.pose.pose.orientation.z,msg.pose.pose.orientation.w)