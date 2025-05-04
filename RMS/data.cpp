#include "data.h"

/*
    User:
    bool isAdmin;
    QString username;
    QString password;

    int favorite_recipes_num = 0;
    int favorite_recipes[100];  // IDs of favorite recipes

    QString name;
    int my_recipes_num = 0;
    QSharedPointer<Recipe> my_recipes[100];
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
    QString imagePath = "";

    int ing_num = 0;
    QString ingredients[100];

   int steps_num = 0;
   QString steps[100] {};
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
        if (recipes[i] == nullptr)
        {
            continue;
        }

        stream << recipes[i]->id << Qt::endl;
        stream << recipes[i]->category << Qt::endl;
        stream << recipes[i]->title << Qt::endl;
        stream << recipes[i]->description << Qt::endl;
        stream << recipes[i]->cock_time << Qt::endl;
        stream << recipes[i]->level << Qt::endl;
        stream << recipes[i]->imagePath << Qt::endl;

        // save ingredients
        stream << recipes[i]->ing_num << Qt::endl;
        QString ing;
        for (int j = 0; j < recipes[i]->ing_num; j++)
        {
            ing.append(QString("%1,").arg(recipes[i]->ingredients[j]));
        }
        ing.chop(1);
        stream << ing << Qt::endl;

        // save steps
        stream << recipes[i]->steps_num << Qt::endl;
        QString step;
        for (int j = 0; j < recipes[i]->steps_num; j++)
        {
            step.append(QString("%1,").arg(recipes[i]->steps[j]));
        }
        step.chop(1);
        stream << step << Qt::endl;
    }


    file.close();
    //qInfo() << num_of_recipes << "Recipes were saved successfully!";
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
        recipe_ptr->cock_time = stream.readLine().toDouble();
        recipe_ptr->level = stream.readLine().toInt();
        recipe_ptr->imagePath = stream.readLine();

        // load ingredients
        recipe_ptr->ing_num = stream.readLine().toInt();
        QString s = stream.readLine();
        QStringList sl = s.split(",");
        for (int j = 0; j < recipe_ptr->ing_num; j++)
        {
            recipe_ptr->ingredients[j] = sl[j];
        }

        // load steps
        recipe_ptr->steps_num = stream.readLine().toInt();
        s = stream.readLine();
        sl = s.split(",");
        for (int j = 0; j < recipe_ptr->steps_num; j++)
        {
            recipe_ptr->steps[j] = sl[j];
        }
        recipes[i] = recipe_ptr;
        recipes_id_to_index[recipe_ptr->id] = i;
    }

    if (num_of_recipes) next_id = recipes[num_of_recipes - 1]->id + 1;

    file.close();
    qInfo() << file_name << " was loaded successfully!";
}
