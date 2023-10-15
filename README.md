I ventured into an exploration of threading and networking within the realm of C++. This journey presented various unique challenges, which were imperative to overcome to develop a real-time 3D application with networking capabilities.

The Core Engine
While the main concentration was on threading and networking, the foundation of this endeavor was built on a game engine. This game engine wasn’t new to me; it was something I had made before for my simulation. It was an insightful experience because it highlighted some latent issues in the engine's design. These became apparent only when the engine was put to the test in creating a comprehensive game.

Thread Mechanics
The threading mechanism I opted for was a thread pool. This decision effectively frontloaded the overhead of thread creation to the program's commencement, ensuring swift performance when tasks needed to be allocated to threads.

Each thread is essentially a C++ thread housed within a class, with an innate loop that finishes the assigned task and then asks the thread manager for another. 

Networking Nuances
Networking was architectured in a peer-to-peer fashion utilizing Server-client. Winsock2 was used to implement this.

The design encompassed a dedicated server awaiting the Client. Once a connection is perceived, it's channeled to a thread, which then listens for socket messages.

Outgoing messages are relayed to server and client concurrently.

The handling of incoming messages is sequenced, with a system in place that processes them chronologically.

Game Dynamics
Players are greeted with a 3D canvas populated by cubes, each colored distinctively. Players can sketch on this canvas to appropriate another player's cube "weight" or, in other words, their color. This dynamic results in cubes being stolen, modified, and reclaimed, creating an engaging interplay of colors.


Reflection & Way Forward
In retrospect, the project can be deemed a success. The game exhibits consistent performance, networking, and canvas drawing.

![canvas](https://github.com/j4m4lk/CanvasGame/assets/77670357/4b1598b8-cd13-4385-855e-e1014349d49b)

However, some avenues remain for enhancement, notably in the engine's versatility and latency reduction. An immediate area of improvement is the task parameter limit in threading. Introducing dynamic parameter intake, maybe via a vector of void pointers, could be a solution. On the networking front, refining message packaging and optimizing traffic distribution could further elevate performance.
