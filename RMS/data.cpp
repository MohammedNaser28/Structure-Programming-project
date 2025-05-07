<<<<<<< HEAD
#include "data.h"

/*
    User:
    bool isAdmin;
    QString username;
    QString password;

    int favorite_recipes_num = 0;
    int favorite_recipes[100];  // IDs of favorite recipes
*/

void save_users()
{
    // open the file
    QString file_name = "Users.txt";
    QFile file(file_name);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCritical() << "Could not open " << file_name << ":" << file.errorString();
        return;
    }

    // Create a stream
    QTextStream stream(&file);

    // write number of users
    stream << num_of_users << Qt::endl;

    // write each user
    for (int i = 0; i < num_of_users; i++)
    {
        stream << users[i]->isAdmin << Qt::endl;
        stream << users[i]->username << Qt::endl;
        stream << users[i]->password << Qt::endl;

        // we have to make sure that all fav recipes weren't deleted from the system
        QString fav;
        for (int j = 0; j < users[i]->favorite_recipes_num; j++)
        {
            if (recipes_id_to_index[users[i]->favorite_recipes[j]] != -1)
                fav.append(QString("%1 ").arg(users[i]->favorite_recipes[j]));
            else
                users[i]->favorite_recipes_num -= 1;
        }

        stream << users[i]->favorite_recipes_num << Qt::endl;
        if (users[i]->favorite_recipes_num)
        {
            fav.chop(1);
            stream << fav << Qt::endl;
        }
    }

    file.close();
    qInfo() << num_of_users << "Users were saved successfully!";
}

void load_users()
{
    // we won't need to load again after the first log in
    if (num_of_users) return;

    // open the file
    QString file_name = "Users.txt";
    QFile file(file_name);
    if (!file.exists())
    {
        qWarning() << file_name << " does not exist";
        return;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qCritical() << "Could not open " << file_name << ":" << file.errorString();
        file.close();
        return;
    }

    // Create a stream
    QTextStream stream(&file);

    // get number of users
    stream >> num_of_users;

    // append each user to users list
    qInfo() << "loading" << num_of_users << "users...";
    QString temp;
    for (int i = 0; i < num_of_users; i++)
    {
        QSharedPointer<User> user_ptr(new User());

        stream >> temp;
        user_ptr->isAdmin = (temp == "1") ? true : false;

        stream >> user_ptr->username;
        stream >> user_ptr->password;

        stream >> temp;
        user_ptr->favorite_recipes_num = temp.toInt();

        for (int j = 0; j < user_ptr->favorite_recipes_num; j++)
        {
            stream >> temp;
            user_ptr->favorite_recipes[j] = temp.toInt();
        }

        users[i] = user_ptr;
    }

    file.close();
    qInfo() << file_name << " was loaded successfully!";
}



/*
    Recipe:
    int id;
    int category;
    QString title;
    QString description;
    int cock_time;
    int level;
    int rates_sum, rates_num;

    int ing_num = 0;
    QString ingredients[100];
*/

void save_recipes()
{
    // open the file
    QString file_name = "Recipes.txt";
    QFile file(file_name);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCritical() << "Could not open " << file_name << ":" << file.errorString();
        return;
    }

    // Create a stream
    QTextStream stream(&file);

    // write num of recipes
    stream << num_of_recipes << Qt::endl;

    // write each recipe
    for (int i = 0; i < num_of_recipes; i++)
    {
        if (recipes[i] == nullptr) continue;

        stream << recipes[i]->id << Qt::endl;
        stream << recipes[i]->category << Qt::endl;
        stream << recipes[i]->title << Qt::endl;
        stream << recipes[i]->description << Qt::endl;
        stream << recipes[i]->cock_time << Qt::endl;
        stream << recipes[i]->level << Qt::endl;
        stream << recipes[i]->rates_sum << Qt::endl;
        stream << recipes[i]->rates_num << Qt::endl;

        stream << recipes[i]->ing_num << Qt::endl;
        QString ing;
        for (int j = 0; j < recipes[i]->ing_num; j++)
        {
            ing.append(QString("%1,").arg(recipes[i]->ingredients[j]));
        }
        ing.chop(1);
        stream << ing << Qt::endl;
    }


    file.close();
    qInfo() << num_of_recipes << "Recipes were saved successfully!";
}

void load_recipes()
{
    // we won't need to load again after the first log in
    if (num_of_recipes) return;

    // open the file
    QString file_name = "Recipes.txt";
    QFile file(file_name);
    if (!file.exists())
    {
        qWarning() << file_name << " does not exist";
        return;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qCritical() << "Could not open " << file_name << ":" << file.errorString();
        file.close();
        return;
    }

    // Create a stream
    QTextStream stream(&file);

    // get number of recipes
    num_of_recipes = stream.readLine().toInt();

    // append each recipe to recipes list
    qInfo() << "loading" << num_of_recipes << " recipes...";
    for (int i = 0; i < num_of_recipes; i++)
    {
        QSharedPointer<Recipe> recipe_ptr(new Recipe());

        recipe_ptr->id = stream.readLine().toInt();
        recipe_ptr->category = stream.readLine().toInt();
        recipe_ptr->title = stream.readLine();
        recipe_ptr->description = stream.readLine();
        recipe_ptr->cock_time = stream.readLine().toInt();
        recipe_ptr->level = stream.readLine().toInt();
        recipe_ptr->rates_sum = stream.readLine().toInt();
        recipe_ptr->rates_num = stream.readLine().toInt();

        recipe_ptr->ing_num = stream.readLine().toInt();
        QString s = stream.readLine();
        QStringList sl = s.split(",");
        for (int j = 0; j < recipe_ptr->ing_num; j++)
        {
            recipe_ptr->ingredients[j] = sl[j];
        }

        recipes[i] = recipe_ptr;
        recipes_id_to_index[recipe_ptr->id] = i;
    }

    if (num_of_recipes) next_id = recipes[num_of_recipes - 1]->id + 1;

    file.close();
    qInfo() << file_name << " was loaded successfully!";
}
=======
#include "data.h"

/*
    User:
    bool isAdmin;
    QString username;
    QString password;

    int favorite_recipes_num = 0;
    int favorite_recipes[100];  // IDs of favorite recipes
*/

void save_users()
{
    // open the file
    QString file_name = "Users.txt";
    QFile file(file_name);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCritical() << "Could not open " << file_name << ":" << file.errorString();
        return;
    }

    // Create a stream
    QTextStream stream(&file);

    // write number of users
    stream << num_of_users << Qt::endl;

    // write each user
    for (int i = 0; i < num_of_users; i++)
    {
        stream << users[i]->isAdmin << Qt::endl;
        stream << users[i]->username << Qt::endl;
        stream << users[i]->name << Qt::endl;
        stream << users[i]->password << Qt::endl;

        // we have to make sure that all fav recipes weren't deleted from the system
        QString fav;
        for (int j = 0; j < users[i]->favorite_recipes_num; j++)
        {
            if (recipes_id_to_index[users[i]->favorite_recipes[j]] != -1)
                fav.append(QString("%1 ").arg(users[i]->favorite_recipes[j]));
            else
                users[i]->favorite_recipes_num -= 1;
        }

        stream << users[i]->favorite_recipes_num << Qt::endl;
        if (users[i]->favorite_recipes_num)
        {
            fav.chop(1);
            stream << fav << Qt::endl;
        }

        // add my recipes
        // write num of recipes
        stream << users[i]->my_recipes_num << Qt::endl;

        // write each recipe
        for (int c = 0; c < users[i]->my_recipes_num; c++)
        {
            if (users[i]->my_recipes[c] == nullptr) continue;

            stream << users[i]->my_recipes[c]->category << Qt::endl;
            stream << users[i]->my_recipes[c]->title << Qt::endl;
            stream << users[i]->my_recipes[c]->description << Qt::endl;
            stream << users[i]->my_recipes[c]->cock_time << Qt::endl;
            stream << users[i]->my_recipes[c]->level << Qt::endl;
            stream << users[i]->my_recipes[c]->rates_sum << Qt::endl;
            stream << users[i]->my_recipes[c]->rates_num << Qt::endl;
            stream << users[i]->my_recipes[c]->imagePath << Qt::endl;
            stream << users[i]->my_recipes[c]->ing_num << Qt::endl;

            QString ing;
            for (int j = 0; j < users[i]->my_recipes[c]->ing_num; j++)
            {
                ing.append(QString("%1,").arg(users[i]->my_recipes[c]->ingredients[j]));
            }
            ing.chop(1);
            stream << ing << Qt::endl;


            stream << users[i]->my_recipes[c]->steps_num << Qt::endl;

            QString steps;
            for (int j = 0; j < users[i]->my_recipes[c]->steps_num; j++)
            {
                steps.append(QString("%1,").arg(users[i]->my_recipes[c]->steps[j]));
            }
            steps.chop(1);
            stream << steps << Qt::endl;
        }
    }

    file.close();
    qInfo() << num_of_users << "Users were saved successfully!";
}

void load_users()
{
    // we won't need to load again after the first log in
    if (num_of_users) return;

    // open the file
    QString file_name = "Users.txt";
    QFile file(file_name);
    if (!file.exists())
    {
        qWarning() << file_name << " does not exist";
        return;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qCritical() << "Could not open " << file_name << ":" << file.errorString();
        file.close();
        return;
    }

    // Create a stream
    QTextStream stream(&file);

    // get number of users
    stream >> num_of_users;
    stream.readLine(); // Consume the newline after num_of_users

    // append each user to users list
    qInfo() << "loading" << num_of_users << "users...";
    QString temp;
    for (int i = 0; i < num_of_users; i++)
    {
        QSharedPointer<User> user_ptr(new User());

        users[i] = user_ptr;
        stream >> temp;
        user_ptr->isAdmin = (temp == "1") ? true : false;
        stream >> user_ptr->username;
        stream >> user_ptr->name;
        stream >> user_ptr->password;
        stream >> temp;
        user_ptr->favorite_recipes_num = temp.toInt();

        if (user_ptr->favorite_recipes_num > 0) {
            // Read the favorite recipes line
            stream.readLine(); // Consume the newline after favorite_recipes_num
            QString favLine = stream.readLine();
            QStringList favList = favLine.split(" ");

            for (int j = 0; j < user_ptr->favorite_recipes_num; j++) {
                user_ptr->favorite_recipes[j] = favList[j].toInt();
            }
        }

        // Read my_recipes_num
        stream >> user_ptr->my_recipes_num;
        stream.readLine(); // Consume the newline after my_recipes_num

        qInfo() << "loading" << user_ptr->my_recipes_num << " recipes...";
        for (int c = 0; c < user_ptr->my_recipes_num; c++)
        {
            QSharedPointer<Recipe> recipe_ptr(new Recipe());

            recipe_ptr->category = stream.readLine().toInt();
            recipe_ptr->title = stream.readLine();
            recipe_ptr->description = stream.readLine();
            recipe_ptr->cock_time = stream.readLine().toDouble();
            recipe_ptr->level = stream.readLine().toInt();
            recipe_ptr->imagePath = stream.readLine();
            recipe_ptr->ing_num = stream.readLine().toInt();

            QString ingred = stream.readLine();
            QStringList ingredList = ingred.split(",");
            for (int j = 0; j < recipe_ptr->ing_num; j++)
            {
                recipe_ptr->ingredients[j] = ingredList[j];
            }

            recipe_ptr->steps_num = stream.readLine().toInt();
            QString step = stream.readLine();
            QStringList sl = step.split(",");
            for (int j = 0; j < recipe_ptr->steps_num; j++)
            {
                recipe_ptr->steps[j] = sl[j];
            }

            user_ptr->my_recipes[c] = recipe_ptr;
        }
    }

    file.close();
    qInfo() << file_name << " was loaded successfully!";
}

/*
    Recipe:
    int id;
    int category;
    QString title;
    QString description;
    int cock_time;
    int level;
    int rates_sum, rates_num;

    int ing_num = 0;
    QString ingredients[100];
*/

void save_recipes()
{
    // open the file
    QString file_name = "Recipes.txt";
    QFile file(file_name);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCritical() << "Could not open " << file_name << ":" << file.errorString();
        return;
    }

    // Create a stream
    QTextStream stream(&file);

    // write num of recipes
    stream << num_of_recipes << Qt::endl;

    // write each recipe
    for (int i = 0; i < num_of_recipes; i++)
    {
        if (recipes[i] == nullptr) continue;

        stream << recipes[i]->id << Qt::endl;
        stream << recipes[i]->category << Qt::endl;
        stream << recipes[i]->title << Qt::endl;
        stream << recipes[i]->description << Qt::endl;
        stream << recipes[i]->cock_time << Qt::endl;
        stream << recipes[i]->level << Qt::endl;
        stream << recipes[i]->imagePath << Qt::endl;
        stream << recipes[i]->ing_num << Qt::endl;

        QString ing;
        for (int j = 0; j < recipes[i]->ing_num; j++)
        {
            ing.append(QString("%1,").arg(recipes[i]->ingredients[j]));
        }
        ing.chop(1);
        stream << ing << Qt::endl;


        stream << recipes[i]->steps_num << Qt::endl;

        QString steps;
        for (int j = 0; j < recipes[i]->steps_num; j++)
        {
            steps.append(QString("%1,").arg(recipes[i]->steps[j]));
        }
        steps.chop(1);
        stream << steps << Qt::endl;
    }


    file.close();
    qInfo() << num_of_recipes << "Recipes were saved successfully!";
}


void load_recipes()
{
    // we won't need to load again after the first log in
    if (num_of_recipes) return;
    // open the file
    QString file_name = "Recipes.txt";
    QFile file(file_name);
    if (!file.exists())
    {
        qWarning() << file_name << " does not exist";
        return;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qCritical() << "Could not open " << file_name << ":" << file.errorString();
        file.close();
        return;
    }
    // Create a stream
    QTextStream stream(&file);
    // get number of recipes
    num_of_recipes = stream.readLine().toInt();
    // append each recipe to recipes list
    qInfo() << "loading" << num_of_recipes << " recipes...";
    for (int i = 0; i < num_of_recipes; i++)
    {
        QSharedPointer<Recipe> recipe_ptr(new Recipe());
        recipe_ptr->id = stream.readLine().toInt();
        qInfo() << recipe_ptr->id;

        recipe_ptr->category = stream.readLine().toInt();
        qInfo() << recipe_ptr->category << " CATEGORY";


        recipe_ptr->title = stream.readLine();
        qInfo() << recipe_ptr->title << " title";

        recipe_ptr->description = stream.readLine();
        qInfo() << recipe_ptr->description << "  DESC";
        recipe_ptr->cock_time = stream.readLine().toInt();
        qInfo() << recipe_ptr->cock_time << " COCKTIME";

        recipe_ptr->level = stream.readLine().toInt();
        qInfo() << recipe_ptr->level << " LEVEL";



        recipe_ptr->imagePath = stream.readLine();
        qInfo() << recipe_ptr->imagePath << " IMG";

        qInfo() << recipe_ptr->imagePath; 
        recipe_ptr->ing_num = stream.readLine().toInt();
        QString ingred = stream.readLine();
        QStringList ingredList = ingred.split(",");

        // Make sure we don't try to access more elements than exist in the list
        int actualIngCount = qMin(recipe_ptr->ing_num, ingredList.size());
        for (int j = 0; j < actualIngCount; j++)
        {
            if (j < 100) { // Your Recipe struct has arrays of size 100
                recipe_ptr->ingredients[j] = ingredList.at(j);
            }
        }

        recipe_ptr->steps_num = stream.readLine().toInt();
        QString step = stream.readLine();
        QStringList stepList = step.split(",");

        // Make sure we don't try to access more elements than exist in the list
        int actualStepsCount = qMin(recipe_ptr->steps_num, stepList.size());
        for (int j = 0; j < actualStepsCount; j++)
        {
            if (j < 100) { // Your Recipe struct has arrays of size 100
                recipe_ptr->steps[j] = stepList.at(j);
            }
        }

        recipes[i] = recipe_ptr;
        recipes_id_to_index[recipe_ptr->id] = i;
    }
    if (num_of_recipes) next_id = recipes[num_of_recipes - 1]->id + 1;
    file.close();
    qInfo() << file_name << " was loaded successfully!";
}
//
//void load_recipes()
//{6
//    // we won't need to load again after the first log in
//    if (num_of_recipes) return;
//
//    // open the file
//    QString file_name = "Recipes.txt";
//    QFile file(file_name);
//    if (!file.exists())
//    {
//        
//        qWarning() << file_name << " does not exist";
//        return;
//    }
//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//    {
//        qCritical() << "Could not open " << file_name << ":" << file.errorString();
//        file.close();
//        return;
//    }
//
//    // Create a stream
//    QTextStream stream(&file);
//
//    // get number of recipes
//    num_of_recipes = stream.readLine().toInt();
//
//    // append each recipe to recipes list
//    qInfo() << "loading" << num_of_recipes << " recipes...";
//    for (int i = 0; i < num_of_recipes; i++)
//    {
//        QSharedPointer<Recipe> recipe_ptr(new Recipe());
//
//        recipe_ptr->id = stream.readLine().toInt();
//        qInfo() << recipe_ptr->id<<'\n';
//        recipe_ptr->category = stream.readLine().toInt();
//        qInfo() << recipe_ptr->category << '\n';
//
//        recipe_ptr->title = stream.readLine();
//        qInfo() << recipe_ptr->title << '\n';
//
//        recipe_ptr->description = stream.readLine();
//        qInfo() << recipe_ptr->description << '\n';
//
//        recipe_ptr->cock_time = stream.readLine().toDouble();
//        qInfo() << recipe_ptr->cock_time << '\n';
//
//        recipe_ptr->level = stream.readLine().toInt();
//        qInfo() << recipe_ptr->level << '\n';
//
//        recipe_ptr->imagePath = stream.readLine();
//        qInfo() << recipe_ptr->imagePath;
//        recipe_ptr->ing_num = stream.readLine().toInt();
//        qInfo() << recipe_ptr->ing_num << '\n';
//
//        QString ingred = stream.readLine();
//        QStringList ingredList = ingred.split(",");
//        for (int j = 0; j < recipe_ptr->ing_num; j++)
//        {
//            recipe_ptr->ingredients[j] = ingredList[j];
//            qInfo() << recipe_ptr->ingredients[j] << '\n';
//
//        }
//
//        recipe_ptr->steps_num = stream.readLine().toInt();
//        QString step = stream.readLine();
//        QStringList sl = step.split(",");
//        for (int j = 0; j < recipe_ptr->steps_num; j++)
//        {
//            recipe_ptr->steps[j] = sl[j];
//            qInfo() << recipe_ptr->steps[i] << '\n';
//
//        }
//
//        recipes[i] = recipe_ptr;
//        recipes_id_to_index[recipe_ptr->id] = i;
//    }
//
//    if (num_of_recipes) next_id = recipes[num_of_recipes - 1]->id + 1;
//
//    file.close();
//    qInfo() << file_name << " was loaded successfully!";
//}
>>>>>>> gui
