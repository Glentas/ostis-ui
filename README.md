# Installation & Build

## ostis-ui

#### 1. Setup venv and conan

The project uses Conan to manage dependencies. 
Create venv, activate it and install conan.

```bash
python3 -m venv .venv
source .venv/bin/activate
pip3 install conan
```
#### 2. Clone Repository

First, clone the repository containing the ostis-ui:

```bash
git clone git@github.com:Glentas/ostis-ui.git
cd ostis-ui
git checkout demo
```

#### 3. Install Dependencies with Conan

Conan stuff can be found in ```/home/<Your_pc_name>/.conan2/```.

Sc-machine files can be found in: 
```/home/<Your_pc_name>/.conan2/p/sc-<something-something>/es/```.

Perform all commands below:

```bash
conan profile detect
conan remote add ostis-ai https://conan.ostis.net/artifactory/api/conan/ostis-ai-library
```

```bash
conan install . -s build_type=Release --build=missing
```

```bash
conan install . -s build_type=Debug --build=missing
```
#### 4. Configure Project

You can configure the project using CMake presets. 


After You installed all dependencies there are three main configuration options:
- Debug with tests:
  
  ```sh
  cmake --preset debug-conan
  ```

- Release:
  
  ```sh
  cmake --preset release-conan
  ```

- Release with tests:
  
  ```sh
  cmake --preset release-with-tests-conan
  ```

#### 5. Build Project

After configuring, You can build the project:

For debug build:

```sh
cmake --build --preset debug
```

For release build:

```sh
cmake --build --preset release
```

## sc-machine

#### 1. Download and extract

Download [GitHub Releases](https://github.com/ostis-ai/sc-machine/releases) and extract them to a location of Your choice.

#### 2. Build KB

Go to ```/path/to/extracted/machine/sc-machine-0.10.5-Linux/bin/```.

Build KB:

```bash
./sc-builder --input /path/to/folder/with/kb/files/ --output /path/to/kb.bin --clear
```

- ```/path/to/folder/with/kb/files/``` - folder that contains Your gwf's and scs's.
- ```/path/to/kb.bin``` - location where KB will be saved. You may change it's name: kb1.bin, my_kb.bin, etc.

#### 3. Start sc-machine

```bash
./sc-machine -s /path/to/kb.bin -e /path/to/ui_libs/build/Release/lib/
```

- ```/path/to/ui_libs/build/Release/lib/``` - location with dynamic ostis-ui libs. Library name is ```libostis-ui-html-translator.so```.

# Whole idea behind this realisation

## Recursive realisation of ui components

Let's think of each UI component as of self-sustainable component. Which means that button, whole div container, whole ui html document, script with javascript code or single color value (#000022) for some paragraph are equal and have the same structure.

Now that we can treat this components equally we can easily design recursive model.

For example:
We start from root - our whole document Its template may look like this:
```html
<!DOCTYPE html>
<html lang="en">
  <head>
    {{general_head1}}
  </head>
  <body>
    {{custom_body1}}
  </body>
</html>
```
We can see that there are 2 parameters: *general_head1* and *custom_body1*.
Model, designed in SCg, leads us for value for these parameters.

Lucky for us *general_head1* already have had HTML representation:

```html
<meta charset="UTF-8" />
<meta name="viewport" content="width=device-width, initial-scale=1.0, viewport-fit=cover" />
<title>Format converter</title>
<style>
  body {
    margin: 0;
    min-height: auto;
    display: flex;
    justify-content: center;
    align-items: center;
  }
</style>
```

Now we can complete part of our root document:

```html
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0, viewport-fit=cover" />
    <title>Format converter</title>
    <style>
      body {
        margin: 0;
        min-height: auto;
        display: flex;
        justify-content: center;
        align-items: center;
      }
    </style>
  </head>
  <body>
    {{custom_body1}}
  </body>
</html>
```

However *custom_body1* doesn't have completed HTML representation. It only has, well, template again:
```html
{{paragraph_of_mine_yeah}}
{{textarea_of_mine_yeah}}
```

Now we need to find *paragraph_of_mine_yeah* and *textarea_of_mine_yeah* elements in database and insert them in this template.

As You can see this gives us simple recucursive task.
But You may be wondering: "When does recursion stops? What if lower elements don't have HTML representation? How will recursion stop?"

Well, You see, the standart for this model **requires** that all component-leaves must have HTML representation.
Look at example below:

```html
<textarea
  id={{textarea_id1}}
  placeholder="{{placeholder1}}
  style="
    width: {{width1}};
    height: {{height1}};
    padding: {{padding2}};
    font-family: {{ff2}};
    font-size: {{fz2}};
    border: {{brd2}};
    border-radius: {{brd_radius1}};
    background: {{bg1}};
    resize: {{resize1}};
    margin: {{margin1}};
  "
>
</textarea>
```

*width: {{width1}}*, *height: {{height1}}*, ... are in fact component-leaves. So they do have HTML represenation by default: *100%*, *auto*, ...

