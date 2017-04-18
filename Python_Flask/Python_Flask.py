from flask import Flask, render_template

app = Flask(__name__)

@app.route("/")
@app.route("/<user>")
def index(user=None):
    return render_template("user.html",user=user)


@app.route("/shopping")
def shopping():
    food=["Cheese","Burger","Milk"]
    return render_template("shopping.html",food=food)

@app.route('/profile/<name>')
def profile(name):
    return render_template("profile.html",names=name)



if __name__ == '__main__':
    app.run(debug=True)
