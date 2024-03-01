#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		sAppName = "BartenderVN";
	}
	enum NpcsEnum
	{
		drifter = 0,
		officelady = 1,
		trucker = 2,
		thug = 3,
		marriedman = 4,
		marriedwoman = 5,
		changling = 6,
	};
	struct Rect
	{
		olc::vf2d pos;
		olc::vf2d size;
	};
	struct mapTiles
	{
		Rect tileRect;
		bool collidable;
		olc::Sprite* tileSpritePtr;
		olc::Decal* tileDecPtr;

	};
	std::vector<std::vector<mapTiles>> Map;
	struct Character
	{
		Rect overworldRect;
		olc::vf2d PortraitPos;
		olc::Sprite* PortraitsprPtr;
		olc::Decal* PortraitDecptr;
		olc::Sprite* overWorldSprPtr;
		olc::Decal* overWorldDecPtr;
	};
	struct npc
	{
		olc::vf2d PortraitPos;
		Rect overWorldRect;
		olc::Sprite* PortraitsprPtr;
		olc::Decal* PortraitDecptr;
		olc::Sprite* overWorldSprPtr;
		olc::Decal* overWorldDecPtr;
		int IDnum;
		std::string favoriteDrink;
		std::string Problem;
		int  CompletionPointsTotal = 0;;
		int MaxCompletionPoints = 100;
		int CompletionPointsInc = 25;
		bool Changling = false;
		bool problemComplete = false;
		void incrementComplete()
		{
			if (problemComplete == false)
			{
				if (CompletionPointsTotal < MaxCompletionPoints)
				{
					CompletionPointsTotal += CompletionPointsInc;
				}
				else
				{
					CompletionPointsTotal = MaxCompletionPoints;
					problemComplete = true;
				}
			}
		}
	};
	Character MainCharacter;
	std::vector <npc>NPCs;
	struct checkBox
	{
		Rect box;
		Rect innerBox;
		bool checked = false;
		std::string buttontitle;
		float stringScale;
		olc::vf2d stringPos;

		void setinnerBox()
		{
			innerBox.pos = olc::vf2d(box.pos.x + 2, box.pos.y + 2);
			innerBox.size = olc::vf2d(box.size.x - 4, box.size.y - 4);
		}
		void setTitle(std::string title, olc::vf2d pos, float scale = 1.25)
		{
			buttontitle = title;
			stringPos = pos;
			stringScale = scale;
		}
	};
	struct windowBox
	{
		Character* MainChar;
		npc* NpcInScene;
		Rect boxOuter;
		Rect boxInner;
		olc::Pixel outerColor;
		olc::Pixel innerColor;
		std::string displayString;
		olc::vf2d StringPos;
		bool selectionMode = false;
		std::vector<std::string> selectionStrings;
		std::vector<olc::vf2d> selectionStringPos;
		std::vector<checkBox> Selections;
		void setBox(olc::vf2d pos, olc::vf2d size, olc::Pixel colorinner, olc::Pixel colorOuter)
		{
			boxInner.pos = pos;
			boxInner.size = size;
			setBorder();

			innerColor = colorinner;
			outerColor = colorOuter;

		}
		void setBorder()
		{
			boxOuter.pos = { boxInner.pos.x - 4, boxInner.pos.y - 4 };
			boxOuter.size = { boxInner.size.x + 8, boxInner.size.y + 8 };
		}
		void setString(std::string inputString)
		{
			displayString = inputString;
		}
		void setActors(Character* thisChar,npc* thisNpc )
		{
			MainChar = thisChar;
			NpcInScene = thisNpc;
		}
		void setStringPos()
		{
			StringPos = { boxInner.pos.x + 5,boxInner.pos.y + 5 };
		}
		void setSelections(std::vector<std::string> theseStrings, int numofSelections)
		{
			int x = 0;
			selectionStrings.clear();
			selectionStrings = theseStrings;
			Selections.clear();
			Selections.resize(numofSelections);
			selectionStringPos.clear();
			selectionStringPos.resize(numofSelections);
			for (auto i = Selections.begin(); i < Selections.end(); i++)
			{
				i->box.pos = { boxInner.pos.x + 10,boxInner.pos.y + 10 + 10 * x };
				x++;
			}
			x = 0;
			for (auto i = selectionStringPos.begin(); i < selectionStringPos.end(); i++)
			{
				*i = { Selections[x].box.pos.x + Selections[x].box.size.x + 10,Selections[x].box.pos.y + Selections[x].box.size.y / 2 };
				x++;
			}
		}
	};
	struct cutScene
	{
		windowBox SceneWindow;
		std::vector<std::string> npcStrings;
		std::vector<std::string> MainCharStrings;
		int talkState= 0;
		bool Over = false;
		void toggleTalkState()
		{
			if (talkState == 0)
			{
				talkState = 1;
			}
			else
			{
				talkState = 0;
			}
		}
		void NextStringSetter()
		{
			switch (talkState)
			{
			case 0:
				setNextCharacterString(MainCharStrings[0]);
				MainCharStrings.erase(MainCharStrings.begin());

				break;
			case 1:
				setNextNpcString(npcStrings[0]);
				npcStrings.erase(npcStrings.begin());
				break;
			default:
				break;
			}
		}
		void setNextNpcString(std::string thisString)
		{
			SceneWindow.setString(thisString);
		}
		void setNextCharacterString(std::string thisString)
		{
			SceneWindow.setString(thisString);
		}
		void setStringVecs(int npcNum, int sceneNum, std::string filepath,bool mainChar = false)
		{
			if (mainChar == false)
			{
				std::istringstream textFile(filepath);
				std::string fileString;
				std::string searchString = "";
				std::string textString;
				//name = speakerName;
				//textFile.open(filepath);
				//std::ifstream file(filepath);
				//if (textFile.is_open())
				//{


				while (std::getline(textFile, fileString))
				{
					if (fileString.find(searchString) != std::string::npos)
					{
						size_t pos = fileString.find(searchString);
						if (pos != std::string::npos)
						{
							fileString.erase(pos, searchString.length());
						}

						textString = fileString;
					}
					MainCharStrings.emplace_back(textString);

				}
			}
			else
			{
				std::istringstream textFile(filepath);
				std::string fileString;
				std::string searchString = "";
				std::string textString;
				//name = speakerName;
				//textFile.open(filepath);
				//std::ifstream file(filepath);
				//if (textFile.is_open())
				//{


				while (std::getline(textFile, fileString))
				{
					if (fileString.find(searchString) != std::string::npos)
					{
						size_t pos = fileString.find(searchString);
						if (pos != std::string::npos)
						{
							fileString.erase(pos, searchString.length());
						}

						textString = fileString;
					}
					npcStrings.emplace_back(textString);
					//heyt
				}
			}
		}

	};
	std::vector<std::vector<cutScene>> Scenes;
	cutScene* CurrentScenePtr;
	std::vector<std::string> Drinks;
	std::vector<std::string> Problems;
	std::vector<std::string> sprStrings;
	std::vector<int> npcNums = { drifter,officelady,trucker,thug,marriedman,marriedwoman};
public:
	void DrawCutscene()
	{
		DrawRectDecal(CurrentScenePtr->SceneWindow.boxOuter.pos, CurrentScenePtr->SceneWindow.boxOuter.size);
		FillRectDecal(CurrentScenePtr->SceneWindow.boxInner.pos, CurrentScenePtr->SceneWindow.boxInner.size,olc::BLUE);
		DrawStringDecal(CurrentScenePtr->SceneWindow.StringPos, CurrentScenePtr->SceneWindow.displayString);
		DrawDecal(CurrentScenePtr->SceneWindow.MainChar->PortraitPos, CurrentScenePtr->SceneWindow.MainChar->PortraitDecptr);
		DrawDecal(CurrentScenePtr->SceneWindow.NpcInScene->PortraitPos, CurrentScenePtr->SceneWindow.NpcInScene->PortraitDecptr);
	}
	void InitDrinks(std::string filepath)
	{
		std::istringstream textFile(filepath);
		std::string fileString;
		std::string searchString = "";
		std::string textString;
	//	name = speakerName;
		//textFile.open(filepath);
		//std::ifstream file(filepath);
		//if (textFile.is_open())
		//{


		while (std::getline(textFile, fileString))
		{
			if (fileString.find(searchString) != std::string::npos)
			{
				size_t pos = fileString.find(searchString);
				if (pos != std::string::npos)
				{
					fileString.erase(pos, searchString.length());
				}

				textString = fileString;
			}
			Drinks.emplace_back(textString);


		}
	}
	void InitProblems(std::string filepath)
	{
		std::istringstream textFile(filepath);
		std::string fileString;
		std::string searchString = "";
		std::string textString;
		//name = speakerName;
		//textFile.open(filepath);
		//std::ifstream file(filepath);
		//if (textFile.is_open())
		//{


		while (std::getline(textFile, fileString))
		{
			if (fileString.find(searchString) != std::string::npos)
			{
				size_t pos = fileString.find(searchString);
				if (pos != std::string::npos)
				{
					fileString.erase(pos, searchString.length());
				}

				textString = fileString;
			}
			Problems.emplace_back(textString);

		}
	}
	void InitCutscenes(int npcnum,int scenenum)
	{
		Scenes.resize(npcnum);
		for (auto i = Scenes.begin(); i < Scenes.end(); i++)
		{
			for (int x = 0; x < scenenum; x++)
			{
				cutScene newcutscene;
				i->emplace_back(newcutscene);
			}
		}
		int npC = 0;
		int scenenum = 0;
		for (auto i = Scenes.begin(); i < Scenes.end(); i++)
		{
			for (auto x = i->begin(); x < i->end(); x++)
			{
				
				x->SceneWindow.NpcInScene = &NPCs[npC];
				x->SceneWindow.MainChar = &MainCharacter;
				x->setStringVecs(x->SceneWindow.NpcInScene->IDnum, scenenum,"");
				x->setStringVecs(x->SceneWindow.NpcInScene->IDnum, scenenum, "",true);
				x->NextStringSetter();
				x->SceneWindow.setBox({ 5,ScreenHeight() - 150 }, { ScreenWidth() - 15,125 }, olc::BLUE, olc::WHITE);
				
			}
			npC++;
			scenenum++;
		}
	}
	void InitNpcs(int NpcNum,std::string sprString)
	{
		npc newNpc;
		newNpc.IDnum = NpcNum;
		int randInt = rand() % Drinks.size() - 1;
		newNpc.favoriteDrink = Drinks[randInt];
		randInt = rand() % Problems.size() - 1;
		newNpc.Problem = Problems[randInt];
		newNpc.PortraitsprPtr = new olc::Sprite(sprString);
		newNpc.PortraitDecptr = new olc::Decal(newNpc.PortraitsprPtr);
		NPCs.emplace_back(newNpc);
	}
	void InitSprStrings(std::string filepath)
	{
		std::istringstream textFile(filepath);
		std::string fileString;
		std::string searchString = "";
		std::string textString;
		//name = speakerName;
		//textFile.open(filepath);
		//std::ifstream file(filepath);
		//if (textFile.is_open())
		//{


		while (std::getline(textFile, fileString))
		{
			if (fileString.find(searchString) != std::string::npos)
			{
				size_t pos = fileString.find(searchString);
				if (pos != std::string::npos)
				{
					fileString.erase(pos, searchString.length());
				}

				textString = fileString;
			}
			sprStrings.emplace_back(textString);

		}
	}
	void InitThings()
	{
		for (int i = 0; i < npcNums.size(); i++)
		{
			InitNpcs(npcNums[i], "");
		}
	}
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::BLACK);
		return true;
	}
};


int main()
{
	srand(time(0));
	Example demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();

	return 0;
}
