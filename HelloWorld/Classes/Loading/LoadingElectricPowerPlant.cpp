//**************************************************
/*
 该文档用于把冗长的精灵创建代码拉到一个助教不能轻易看见的地方
 //你的代码怎么那么乱
 */
//**************************************************
#include "../Scene/GameScene.h"


void GameScene::electricPowerPlantMoveOnce(Sprite* ArmyName)
{
    auto ArmyListener = EventListenerTouchOneByOne::create();//创建一个触摸监听
    ArmyListener->setSwallowTouches(true);
    ArmyListener->onTouchBegan = [](Touch* touch, Event* event)
    {
        auto ArmyTarget = static_cast<Sprite*>(event->getCurrentTarget());//获取的当前触摸的目标
        Point locationInNode = ArmyTarget->convertToNodeSpace(touch->getLocation());//获得触摸点相对于触摸目标的坐标
        Size ArmySize = ArmyTarget->getContentSize();//获得触摸目标的大小
        Rect ArmyRect = Rect(0, 0, ArmySize.width, ArmySize.height);//创建一个坐标在左下角的相对于触摸目标的坐标系
        if (ArmyRect.containsPoint(locationInNode))//判断触摸点是否在目标的范围内
            return true;
        else
            return false;
    };
    ArmyListener->onTouchMoved = [](Touch* touch, Event* event)
    {
        auto ArmyTarget = static_cast<Sprite*>(event->getCurrentTarget());
        ArmyTarget->setPosition(ArmyTarget->getPosition() + touch->getDelta());
    };
    
    ArmyListener->onTouchEnded = [=](Touch* touch, Event* event)
    {
        auto ArmyTarget = static_cast<Sprite*>(event->getCurrentTarget());//获取的当前触摸的目标
        Point LocationInWorld = this->convertToNodeSpace(touch->getLocation());//获得触摸点相对于世界地图的坐标
        Size ArmySize = ArmyTarget->getContentSize();
        int x = LocationInWorld.x - ArmySize.width/2;
        int y = LocationInWorld.y - ArmySize.height/2;
        for (int i = 0; i <= ArmySize.width; i++)
        {
            for (int j = 0; j <= ArmySize.height; j++)
            {
                if (MyData.IsPositionHaveBuildings[x+i][y+j] == 1 && x+i >= 0 && y+j >= 0 && x+i <= 1600 && y+j <= 1600)
                {
                    this->removeChild(ArmyName);
                    _eventDispatcher->removeEventListener(ArmyListener);
                    return false;
                }
            }
        }
        MyData.TagNumber += 2;
        for (int i = 0; i <= ArmySize.width; i++)
        {
            for (int j = 0; j <= ArmySize.height; j++)
            {
                if (x+i >= 0 && y+j >= 0 && x+i <= 1600 && y+j <= 1600)
                {
                    MyData.IsPositionHaveBuildings[x+i][y+j] = 1;
                    MyData.PositionTag[x+i][y+j] = MyData.TagNumber-1;
                }
            }
        }
        _eventDispatcher->removeEventListener(ArmyListener);
        MyData.LastTouchPosition = this->convertToNodeSpace(touch->getLocation());
        MyData.IsTouchPositionAvailable = 1;
        this->removeChild(ArmyName);
        electricPowerPlantBuildCallBack();
        MyData.MyMoney -= 50;
    };
    //将触摸监听添加到eventDispacher中去
    _eventDispatcher->addEventListenerWithSceneGraphPriority(ArmyListener, ArmyName);
}



void GameScene::electricPowerPlantBuildCallBack()
{
    auto BuildingSprite = BuildingsClass::createWithSpriteFileName("Common/camera.png");
    BuildingSprite->setPosition(MyData.LastTouchPosition);
    this->addChild(BuildingSprite,1);
    BuildingSprite->runAction(loadingElectricPowerPlantAction());
    MyData.IsTouchPositionAvailable = 0;
    MyData.MyElectricPowerPlant.push_back(BuildingSprite);
    auto BuildingHPBar = LoadingBar::create("GamePicture/HPBar.png");
    BuildingHPBar->setDirection(LoadingBar::Direction::LEFT);
    BuildingHPBar->setScale(0.07f);
    BuildingHPBar->setPercent(100);
    BuildingHPBar->setPosition(Vec2(MyData.LastTouchPosition.x,MyData.LastTouchPosition.y+40));
    this->addChild(BuildingHPBar,2);
    auto HPBarDelay = DelayTime::create(21.0f);
    auto HPBarFadeIn = FadeIn::create(0.0f);
    auto HPBarFadeOut = FadeOut::create(0.0f);
    auto HPBarSequence = Sequence::create(HPBarFadeOut,HPBarDelay,HPBarFadeIn,NULL);
    BuildingHPBar->runAction(HPBarSequence);
    BuildingSprite->setHP(BuildingHPBar);
    BuildingSprite->setHPInterval(100.0f/BuildingSprite->getLifeValue());
    BuildingSprite->setTag(MyData.TagNumber-1);
    BuildingHPBar->setTag(MyData.TagNumber);
}



Action* GameScene::loadingElectricPowerPlantAction()                                                    ////完工
{
    auto BuildingAnimation = Animation::create();
    for (int i = 1; i < 16; i++)
    {
        BuildingAnimation->addSpriteFrameWithFile
        (StringUtils::format("ArmyAction/CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_%d.png",i));
    }
    BuildingAnimation->setDelayPerUnit(3.0f/2.0f);
    auto BuildingAction = Animate::create(BuildingAnimation);
    
    auto NormalAnimation = Animation::create();
    NormalAnimation->addSpriteFrameWithFile
    ("ArmyAction/CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_11.png");
    NormalAnimation->addSpriteFrameWithFile
    ("ArmyAction/CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_12.png");
    NormalAnimation->addSpriteFrameWithFile
    ("ArmyAction/CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_13.png");
    NormalAnimation->addSpriteFrameWithFile
    ("ArmyAction/CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_14.png");
    NormalAnimation->setDelayPerUnit(3.0f/3.0f);
    NormalAnimation->setLoops(-1);
    auto NormalAction = Animate::create(NormalAnimation);
    
    auto AllAction = Sequence::createWithTwoActions(BuildingAction, NormalAction);
    return AllAction;
}








