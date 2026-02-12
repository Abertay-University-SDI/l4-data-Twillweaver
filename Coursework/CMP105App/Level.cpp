#include "Level.h"

Level::Level(sf::RenderWindow& hwnd, Input& in) :
    BaseLevel(hwnd, in), m_timerText(m_font), m_winText(m_font), m_scoreboardText(m_font)
{
    m_isGameOver = false;

    sf::Vector2f levelSize = { 800.f, 500.f };
    m_levelBounds = { {0.f, 0.f}, {levelSize } };

    // Initialise the Player (Rabbit)
    m_playerRabbit = new Rabbit();

    //old:
    // m_playerRabbit->setPosition({ 40.f, 40.f });  // hardcoded. FOR NOW!

    if (!m_rabbitTexture.loadFromFile("gfx/rabbit_sheet.png")) std::cerr << "no rabbit texture";
    if (!m_sheepTexture.loadFromFile("gfx/sheep_sheet.png")) std::cerr << "no sheep texture";

    m_playerRabbit->setSize({ 32,32 });
    m_playerRabbit->setInput(&m_input);
    m_playerRabbit->setTexture(&m_rabbitTexture);
    m_playerRabbit->setWorldSize(levelSize.x, levelSize.y);

    // Load the level from file
    loadLevel("data/level1.txt", levelSize);

    // old sheep code:
    // Initialise Sheep (initial position AND pointer to the rabbit
   // for (int i = 0; i < 3; i++)
    //{
    //    m_sheepList.push_back(new Sheep(sf::Vector2f(200.f + 100 * i, 400.f - 100 * i), m_playerRabbit));
    //    m_sheepList[i]->setTexture(&m_sheepTexture);
    //    m_sheepList[i]->setSize({ 32,32 });
    //    m_sheepList[i]->setWorldSize(levelSize.x, levelSize.y);
    //}

    // Initialise Timer
    m_gameTimer.restart();

    // UI Setup
    if (!m_font.openFromFile("font/arial.ttf")) {
        std::cerr << "Error loading font" << std::endl;
    }

    m_timerText.setFont(m_font);
    m_timerText.setCharacterSize(24);
    m_timerText.setFillColor(sf::Color::White);

    // "Game Over" text setup
    m_winText.setFont(m_font);
    m_winText.setString("ROUND COMPLETE!");
    m_winText.setCharacterSize(50);
    m_winText.setFillColor(sf::Color::White);
    m_winText.setPosition({ 1000.f, 100.f });  // outside of view

    //old goal:
    // setup goal
    //m_goal.setSize({ 50, 50 });

    m_goal.setFillColor(sf::Color::Blue);

    //m_goal.setPosition({ 250, 250 });
    //m_goal.setCollisionBox({ { 0,0 }, { 50,50 } });

    // old walls:
    // setup walls
    //for (int i = 0; i < 2; i++)
    //{
    //    GameObject wall;
    //    wall.setPosition({ 100.f + i * 600, 100.f});
    //    wall.setSize({ 50,300 });
    //    wall.setFillColor(sf::Color::Black);
    //    wall.setCollisionBox({ { 0,0 }, { 50,300 } });
    //    m_walls.push_back(wall);
    //}

    m_bgFarm.setFillColor(sf::Color::Green);
    m_bgFarm.setSize(levelSize);
    
}

void Level::loadLevel(std::string filename, sf::Vector2f worldSize)
{
    std::ifstream inputFile(filename);
    if (!inputFile.is_open())
    {
        std::cerr << "Could not open level file: " << filename << std::endl;
        return;
    }

    std::string type;
    float x, y, w, h;

    // The while loop pulls the first word (TYPE) from each line
    while (inputFile >> type)
    {

        if (type == "LEVELNAME")
        {
            inputFile >> m_levelName;
        }
        else if (type == "RABBIT")
        {
            inputFile >> x >> y;
            m_playerRabbit->setPosition(sf::Vector2f(x, y));
        }
        else if (type == "SHEEP")
        {
            inputFile >> x >> y;
            Sheep* newSheep = new Sheep(sf::Vector2f(x, y), m_playerRabbit);
            newSheep->setTexture(&m_sheepTexture);
            newSheep->setSize(sf::Vector2f(32, 32));
            newSheep->setWorldSize(worldSize.x, worldSize.y);
            m_sheepList.push_back(newSheep);
        }
        else if (type == "WALL")
        {
            inputFile >> x >> y >> w >> h;
            GameObject wall;
            wall.setPosition(sf::Vector2f(x, y));
            wall.setSize(sf::Vector2f(w, h));
            wall.setFillColor(sf::Color::Black);
            wall.setCollisionBox(sf::FloatRect({ 0.f, 0.f }, { w, h }));
            m_walls.push_back(wall);
        }
        else if (type == "GOAL")
        {
            inputFile >> x >> y;
            // You can also add width and height here if you updated your level1.txt
            m_goal.setPosition(sf::Vector2f(x, y));
            m_goal.setSize(sf::Vector2f(50, 50));
            m_goal.setCollisionBox(sf::FloatRect({ 0.f, 0.f }, { 50.f, 50.f }));
        }
    }

    inputFile.close();
}

void Level::displayScoreboard(float currentTime)
{
    std::string filename = "data/highscores_" + m_levelName + ".txt";
    std::ifstream inFile(filename);

    std::vector<float> scores;
    float tempScore;

    //std::string line;
    //std::string fullBoardText = "PREVIOUS SCORES:\n";
    
    // reading scores into a vector
    if (inFile.is_open())
    {
        while (inFile >> tempScore)
        {
            // Add the score and a newline to keep them in a list
            scores.push_back(tempScore);
        }
        inFile.close();
    }
    
    // sort the scores
    std::sort(scores.begin(), scores.end());

    // build the leaderboard
    std::string fullBoardText = "TOP 5:" + m_levelName + "\n";

    int displayCount = std::min((int)scores.size(), 5);

     for (int i = 0; i < displayCount; i++)
    { 
         std::stringstream ss;
         ss << std::fixed << std::setprecision(2) << scores[i];
        
         fullBoardText += std::to_string(i + 1) + "." + ss.str() + "s";

         // highlight the score the player just got
         if (std::abs(scores[i] - currentTime) < 0.01f)
         {
             fullBoardText += " <-- NEW SCORE!";
         }
         fullBoardText += "\n";
    }

     if (scores.empty()) fullBoardText = "No sccores yet!";

    // Set up the text appearance
    m_scoreboardText.setString(fullBoardText);
    m_scoreboardText.setCharacterSize(20);
    m_scoreboardText.setFillColor(sf::Color::White);

    // Position it relative to the "Round Complete" text
    sf::Vector2f winPos = m_winText.getPosition();
    m_scoreboardText.setPosition({ winPos.x, winPos.y + 80.f });

    // Get the center of the current camera view
    sf::Vector2f viewCenter = m_window.getView().getCenter();

    // Position the "Round Complete" and "Scoreboard" in the middle of the screen
    m_winText.setPosition(sf::Vector2f(viewCenter.x - 200.f, viewCenter.y - 200.f));
    m_scoreboardText.setPosition(sf::Vector2f(viewCenter.x + 50.f, viewCenter.y - 110.f));
}

Level::~Level()
{
	// We made a lot of **new** things, allocating them on the heap
	// So now we have to clean them up
	delete m_playerRabbit;
	for (Sheep* s : m_sheepList)
	{
		delete s;
	}
	m_sheepList.clear();
}

void Level::UpdateCamera()
{
    sf::View view = m_window.getView();
    sf::Vector2f targetPos = m_playerRabbit->getPosition(); 

    sf::Vector2f viewSize = view.getSize();
    sf::Vector2f halfSize = { viewSize.x / 2.f, viewSize.y / 2.f };

    sf::Vector2f newCenter;

    newCenter.x = std::clamp(targetPos.x,
        m_levelBounds.position.x + halfSize.x,
        m_levelBounds.size.x - halfSize.x);

    newCenter.y = std::clamp(targetPos.y,
        m_levelBounds.position.y + halfSize.y,
        m_levelBounds.size.y - halfSize.y);

    view.setCenter(newCenter);
    
    // top-left corner of the current view in world space
    sf::Vector2f viewTopLeft = newCenter - halfSize;
    // Position text at the bottom-left of the current view with a small margin (e.g., 30px)
    float margin = 30.f;
    m_timerText.setPosition({ viewTopLeft.x + margin, viewTopLeft.y + viewSize.y - margin });

    m_window.setView(view);
}

void Level::writeHighScore(float time)
{
    std::ofstream outFile;
    // Open in append mode so we don't wipe previous scores
    std::string filename = "data/highscores_" + m_levelName + ".txt";

    outFile.open(filename, std::ios::app);

    if (outFile.is_open())
    {
        // Use fixed and setprecision for a clean "0.00" format
        outFile << std::fixed << std::setprecision(2) << time << "\n";
        outFile.close();
    }
    else
    {
        std::cerr << "Error: Could not write to data/highscores.txt. Make sure the 'data' folder exists!" << std::endl;
    }
}

bool Level::CheckWinCondition()
{
    for (auto s : m_sheepList) if (s->isAlive()) return false;
    m_winText.setPosition({ 100.f, 100.f });
 
    return true;
}


// handle user input
void Level::handleInput(float dt)
{
	m_playerRabbit->handleInput(dt);
}

// checks and manages sheep-sheep, sheep-wall, sheep-goal, player-wall
void Level::manageCollisions()
{
    for (int i = 0; i < m_sheepList.size(); i++)
    {
        if (!m_sheepList[i]->isAlive()) continue;   // ignore scored sheep.

        // sheep collide with walls, with other sheep and with the goal.
        for (auto wall : m_walls)
        {
            if (Collision::checkBoundingBox(wall, *m_sheepList[i]))
            {
                m_sheepList[i]->collisionResponse(wall);
            }
        }
        for (int j = i + 1; j < m_sheepList.size(); j++)
        {
            if(!m_sheepList[j]->isAlive()) continue; // ignore scored sheep here too
            if (Collision::checkBoundingBox(*m_sheepList[i], *m_sheepList[j]))
            {
                // DANGER check i and j carefully here team.
                m_sheepList[i]->collisionResponse(*m_sheepList[j]);
                m_sheepList[j]->collisionResponse(*m_sheepList[i]);
            }
        }
        if (Collision::checkBoundingBox(*m_sheepList[i], m_goal))
            m_sheepList[i]->collideWithGoal(m_goal);
    }
    for (auto wall : m_walls)
    {
        if (Collision::checkBoundingBox(wall, *m_playerRabbit))
        {
            m_playerRabbit->collisionResponse(wall);
        }
    }
}

// Update game objects
void Level::update(float dt)
{
    if (m_isGameOver) return;   // if the game is over, don't continue trying to process game logic

    m_playerRabbit->update(dt);
    for (Sheep* s : m_sheepList)
    {
        if (s->isAlive()) s->update(dt);
    }

    // Timer 
    float timeElapsed = m_gameTimer.getElapsedTime().asSeconds();
    m_timerText.setString("Time: " + std::to_string(static_cast<int>(timeElapsed)));


    manageCollisions();
    UpdateCamera();

    // Check if the game just ended this frame
    if (CheckWinCondition())
    {
        m_isGameOver = true;
        writeHighScore(timeElapsed);
        displayScoreboard(timeElapsed); // Pass the current time here
    }
 
    //old:
   // m_isGameOver = CheckWinCondition();

}

// Render level
void Level::render()
{
    beginDraw();

    // 1. Draw Environment
    m_window.draw(m_bgFarm);
    m_window.draw(m_goal);
    for (auto w : m_walls) m_window.draw(w);

    // 2. Draw Entities
    for (auto s : m_sheepList)
    {
        if (s->isAlive()) m_window.draw(*s);
    }
    m_window.draw(*m_playerRabbit);

    // 3. Draw UI
    m_window.draw(m_timerText);

    // 4. Draw Game Over Overlay
    if (m_isGameOver)
    {
        m_window.draw(m_winText);
        m_window.draw(m_scoreboardText);
    }

    endDraw();
}


