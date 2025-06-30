#include "data.h"


/*
    Recipe:
    int id;
    int category;
    QString title;
    QString description;
    int cock_time;
    int level;
    QString imagePath = "";

    int ing_count = 0;
    QString ingredients[100];

   int steps_count = 0;
   QString steps[100] {};
*/
void save_recipes(QSharedPointer<Recipe> recipes[], int& recipes_count, QTextStream* stream)
{
    QFile* file = nullptr;
    QString file_name = "Recipes.txt";
    if (stream == nullptr)
    {
        // open the file
        file = new QFile(file_name);
        if (!file->open(QIODevice::WriteOnly | QIODevice::Text)) {
            qCritical() << "Could not open " << file_name << ":" << file->errorString();
            return;
        }

        // Create a stream
        stream = new QTextStream(file);
    }

    // write count of recipes
    *stream << recipes_count << Qt::endl;

    // write each recipe
    for (int i = 0; i < recipes_count; i++)
    {
        *stream << recipes[i]->id << Qt::endl;
        *stream << recipes[i]->category << Qt::endl;
        *stream << recipes[i]->title << Qt::endl;
        *stream << recipes[i]->description << Qt::endl;
        *stream << recipes[i]->cock_time << Qt::endl;
        *stream << recipes[i]->level << Qt::endl;
        *stream << recipes[i]->imagePath << Qt::endl;

        // save ingredients
        *stream << recipes[i]->ing_count << Qt::endl;
        QString ing;
        for (int j = 0; j < recipes[i]->ing_count; j++)
        {
            ing.append(QString("%1,").arg(recipes[i]->ingredients[j]));
        }
        ing.chop(1);
        *stream << ing << Qt::endl;


        // save steps
        *stream << recipes[i]->steps_count << Qt::endl;
        QString step;
        for (int j = 0; j < recipes[i]->steps_count; j++)
        {
            step.append(QString("%1,").arg(recipes[i]->steps[j]));
        }
        step.chop(1);
        *stream << step << Qt::endl;
    }


    if (file != nullptr)
    {
        file->close();
        qInfo() << recipes_count << "Recipes were saved successfully!";
    }
}
void load_recipes(QSharedPointer<Recipe> recipes[], int& recipes_count, QTextStream* stream)
{
    QFile* file = nullptr;
    QString file_name = "Recipes.txt";
    if (stream == nullptr)
    {
        // open the file
        file = new QFile(file_name);
        if (!file->exists() || !file->open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qCritical() << "Could not open " << file_name << ":" << file->errorString();
            file->close();
            return;
        }

        // Create a stream
        stream = new QTextStream(file);
    }

    // get countber of recipes
    recipes_count = stream->readLine().toInt();

    // append each recipe to recipes list
    qInfo() << "loading" << recipes_count << "recipes...";
    for (int i = 0; i < recipes_count; i++)
    {
        QSharedPointer<Recipe> recipe_ptr(new Recipe());
        recipe_ptr->id = stream->readLine().toInt();
        recipe_ptr->category = stream->readLine().toInt();
        recipe_ptr->title = stream->readLine();
        recipe_ptr->description = stream->readLine();
        recipe_ptr->cock_time = stream->readLine().toDouble();
        recipe_ptr->level = stream->readLine().toInt();
        recipe_ptr->imagePath = stream->readLine();

        // load ingredients
        recipe_ptr->ing_count = stream->readLine().toInt();
        QString s = stream->readLine();
        QStringList sl = s.split(",");
        for (int j = 0; j < recipe_ptr->ing_count; j++)
        {
            recipe_ptr->ingredients[j] = sl[j];
        }

        // load steps
        recipe_ptr->steps_count = stream->readLine().toInt();
        s = stream->readLine();
        sl = s.split(",");
        for (int j = 0; j < recipe_ptr->steps_count; j++)
        {
            recipe_ptr->steps[j] = sl[j];
        }
        
        recipes[i] = recipe_ptr;
        if (file != nullptr)
            recipes_id_to_index[recipe_ptr->id] = i;
    }


    if (file != nullptr)
    {
        if (recipes_count) next_id = recipes[recipes_count - 1]->id + 1;
        file->close();
        qInfo() << file_name << " was loaded successfully!";
    }
}



/*
    User:
    bool isAdmin;
    QString username;
    QString password;

    int favorites_count = 0;
    QSharedPointer<Recipe> favorites[100] {};

    QString name;
    int my_recipes_count = 0;
    QSharedPointer<Recipe> my_recipes[100] {};
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

    // write countber of users
    stream << users_count << Qt::endl;

    // write each user
    for (int i = 0; i < users_count; i++)
    {
        stream << users[i]->isAdmin << Qt::endl;
        stream << users[i]->username << Qt::endl;
        stream << users[i]->password << Qt::endl;

        // make sure that there're no deleted system recipes that were in any user's favorite list
        QString fav;
        int deleted = 0;
        for (int j = 0; j < users[i]->favorites_count; j++)
        {
            int id = users[i]->favorites[j]->id;

            if (recipes_id_to_index[id] != -1) fav.append(QString("%1,").arg(id));
            else deleted++;
        }
        users[i]->favorites_count -= deleted;
        stream << users[i]->favorites_count << Qt::endl;
        if (users[i]->favorites_count)
        {
            fav.chop(1);
            stream << fav << Qt::endl;
        }

        // save my_recipes
        save_recipes(users[i]->my_recipes, users[i]->my_recipes_count, &stream);
    }

    file.close();
    qInfo() << users_count << "Users were saved successfully!";
}
void load_users()
{
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

    // get count of users
    users_count = stream.readLine().toInt();

    // append each user to users list
    qInfo() << "loading" << users_count << "users...";
    for (int i = 0; i < users_count; i++)
    {
        QSharedPointer<User> user_ptr(new User());
        users[i] = user_ptr;

        user_ptr->isAdmin = stream.readLine().toInt();
        user_ptr->username = stream.readLine();
        user_ptr->password = stream.readLine();

        user_ptr->favorites_count = stream.readLine().toInt();
        if (user_ptr->favorites_count)
        {
            QString ids = stream.readLine();
            QStringList ids_list = ids.split(",");
            for (int j = 0; j < user_ptr->favorites_count; j++)
            {
                user_ptr->favorites[j] = recipes[recipes_id_to_index[ids_list[j].toInt()]];
            }
        }

        load_recipes(user_ptr->my_recipes, user_ptr->my_recipes_count, &stream);
    }

    file.close();
    qInfo() << file_name << " was loaded successfully!";
}